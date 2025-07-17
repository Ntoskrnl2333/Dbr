#include "dbr.h"

void EncodeString(char *s, unsigned int len, char *out) {
	for (unsigned int i = 0; i < len; i++) {
		char low = s[i] & 0xF, high = (s[i] & 0xF0) >> 4;
		out[i*2] = low + 'a';
		out[i*2+1] = high + 'a';
	}
}

void DecodeString(char *s, unsigned int len, char *out) {
	if (len & 1)
		len--;
	for (unsigned int i = 0; i < len; i += 2) {
		char low = s[i] - 'a', high = s[i + 1] - 'a';
		out[i/2] = low | high << 4;
	}
}

void BackupWorker() {
	started = true;
	std::queue<std::string> fcbuf;

	FILE* fdbr;
	errno_t dbrerr = fopen_s(&fdbr, filepath.c_str(), "w");
	if (dbrerr)
		throw "cannot open file!";

	char* buffer = new char[BUFFER_SIZE], * buffer2 = new char[BUFFER_SIZE * 2];

	std::wstring wpath = std::filesystem::absolute(folderpath).wstring();
	int rootlen = wpath.size();
	int size_needed = WideCharToMultiByte(
		CP_ACP,
		0,
		wpath.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL
	);
	std::string lpath(size_needed, '\0');
	WideCharToMultiByte(
		CP_ACP,
		0,
		wpath.c_str(),
		-1,
		const_cast<char*>(lpath.c_str()),
		size_needed,
		NULL,
		NULL
	);

	EncodeString(lpath.data(), lpath.size(), buffer2);
	fwrite(buffer2, 2, lpath.size(), fdbr);
	fputc('z', fdbr);

	for (const auto& entry : std::filesystem::recursive_directory_iterator(folderpath)) {
		if (!std::filesystem::is_regular_file(entry))
			continue;

		FILE* fp;
		wpath = entry.path().wstring();
		size_needed = WideCharToMultiByte(
			CP_ACP,
			0,
			wpath.c_str(),
			-1,
			NULL,
			0,
			NULL,
			NULL
		);
		lpath = std::string(size_needed,'\0');
		WideCharToMultiByte(
			CP_ACP,
			0,
			wpath.c_str(),
			-1,
			const_cast<char*>(lpath.c_str()),
			size_needed,
			NULL,
			NULL
		);
		errno_t err = fopen_s(&fp, lpath.c_str(), "rb");

		/*if (err)
			throw "failed to open file!";*/
		if (err)
			continue;

		memcpy(buffer, lpath.c_str() + rootlen, lpath.size() - rootlen);
		EncodeString(buffer, lpath.size() - rootlen, buffer2);
		fwrite(buffer2, 2, lpath.size() - rootlen, fdbr);

		fputc('z', fdbr);

		int curlen;
		while (curlen = fread(buffer, 1, BUFFER_SIZE, fp)) {
			EncodeString(buffer, curlen, buffer2);
			int ret = fwrite(buffer2, 2, curlen, fdbr);
			/*if (ret != curlen)
				throw "ret != curlen !!!";*/
			bytescopied += curlen;
		}

		fputc('z', fdbr);

		fclose(fp);

		fcbuf.push(entry.path().string());
		if (!filecopied_lock) {
			filecopied_lock = true;
			while (!fcbuf.empty()) {
				filecopied.push(fcbuf.front());
				fcbuf.pop();
			}
			filecopied_lock = false;
		}
	}
	delete[] buffer;
	delete[] buffer2;
	working = false;
}

void RestoreWorker() {
	started = true;

	FILE *fdbr, *fp = NULL;
	errno_t err = fopen_s(&fdbr, filepath.c_str(), "r");
	if (err)
		throw "cannot open file!";

	char* buffer = new char[BUFFER_SIZE], *buffer2 = new char[BUFFER_SIZE + 1], *buffer3 = new char[BUFFER_SIZE + 1];
	std::queue<std::string> fcbuf;
	int curlen;
	char lastchar = '\0';
	std::string orig, root = folderpath, curname;

	RESTORE_STATE rs = RS_ROOT;

	while (curlen = fread(buffer, 1, BUFFER_SIZE, fdbr)) {
		char* bufptr = buffer;
		while (curlen > 0) {
			int split = std::find(bufptr, bufptr + curlen, 'z') - bufptr;

			switch (rs) {
			case RS_ROOT:
				if (lastchar == '\0'){
					memcpy(buffer2, bufptr, split);
					DecodeString(buffer2, split, buffer3);
					if (split & 1)
						lastchar = buffer2[split - 1], buffer3[(split - 1) >> 1] = '\0';
					else
						buffer3[split >> 1] = '\0';
				}
				else {
					memcpy(buffer2+1, bufptr, split);
					buffer2[0] = lastchar;
					lastchar = '\0';
					DecodeString(buffer2, split + 1, buffer3);
					if (split + 1 & 1)
						lastchar = buffer2[split], buffer3[split >> 1] = '\0';
					else
						buffer3[(split + 1) >> 1] = '\0';
				}
				orig += buffer3;
				break;
			case RS_NAME:
				if (lastchar == '\0') {
					memcpy(buffer2, bufptr, split);
					DecodeString(buffer2, split, buffer3);
					if (split & 1)
						lastchar = buffer2[split - 1], buffer3[(split - 1) >> 1] = '\0';
					else
						buffer3[split >> 1] = '\0';
				}
				else {
					memcpy(buffer2 + 1, bufptr, split);
					buffer2[0] = lastchar;
					lastchar = '\0';
					DecodeString(buffer2, split + 1, buffer3);
					if (split + 1 & 1)
						lastchar = buffer2[split], buffer3[split >> 1] = '\0';
					else
						buffer3[(split + 1) >> 1] = '\0';
				}
				curname += buffer3;
				break;
			case RS_DATA:
				if (lastchar == '\0') {
					memcpy(buffer2, bufptr, split);
					DecodeString(buffer2, split, buffer3);
					if (split & 1)
						lastchar = buffer2[split - 1], buffer3[(split - 1) >> 1] = '\0';
					else
						buffer3[split >> 1] = '\0';
				}
				else {
					memcpy(buffer2 + 1, bufptr, split);
					buffer2[0] = lastchar;
					lastchar = '\0';
					DecodeString(buffer2, split + 1, buffer3);
					if (split + 1 & 1)
						lastchar = buffer2[split], buffer3[split >> 1] = '\0';
					else
						buffer3[(split + 1) >> 1] = '\0';
				}
				int b3len = std::find(buffer3, buffer3 + BUFFER_SIZE, '\0') - buffer3;
				fwrite(buffer3, 1, b3len, fp);
				bytescopied += b3len;
				break;
			}

			if (split != curlen) {
				switch (rs) {
				case RS_ROOT:
					if (root.empty())
						root = orig;
					rs = RS_NAME;
					break;
				case RS_NAME:
					std::filesystem::create_directories(std::filesystem::path(root+curname).remove_filename());
					err = fopen_s(&fp, (root + curname).c_str(), "wb");
					if (err)
						fprintf(stderr,"fail to open %s\n", (root + curname).c_str());
					rs = RS_DATA;
					break;
				case RS_DATA:
					fclose(fp);
					fcbuf.push(root + curname);
					curname.clear();
					rs = RS_NAME;
					break;
				}
			}

			bufptr += split + 1;
			curlen -= split + 1;
		}
	}
	delete[] buffer;
	delete[] buffer2;
	delete[] buffer3;
	working = false;
}