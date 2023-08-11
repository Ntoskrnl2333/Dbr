#include "Dbr.h"

void __DataBase__::Read(){
	FILE* fp = fopen(fname.c_str(), "rb");
	if (fp == NULL) {
		char *tmp=new char[64];
		sprintf(tmp,"Cannot open \"%s\"!",fname.c_str());
		msgq.push(tmp);
		return;
	}

	char tmps[1024];
	memset(tmps, 0, sizeof(tmps));

	if (fscanf(fp, "%s", tmps) != 1) {
		msgq.push("File format error!");
		fclose(fp);
		return;
	}
	if(root.empty())
		root = SpecSlash(tmps,false);

	if (fread(&num, sizeof(uint), 1, fp)!=sizeof(uint) ){
		msgq.push("File format error!");
		fclose(fp);
		return;
	}

	for (uint i = 0; i <= num; i++) {
		__FileData__ fd;

		memset(tmps, 0, sizeof(tmps));
		if (fscanf(fp, "%s", tmps) != 1) {
			msgq.push("File format error!");
			fclose(fp);
			return;
		}
		
		fd.path = root+SpecSlash(tmps,true);

		if (fread(&fd.size, sizeof(uint), 1, fp) != sizeof(uint)) {
			msgq.push("File format error!");
			fclose(fp);
			return;
		}

		fd.Allocate();
		fd.Read();

		if (fd.err != 0) {
			fd.Release();
			fclose(fp);
			return;
		}
		file.push_back(fd);
	}

	fclose(fp);
	return;
}

void __DataBase__::Write(){
	FILE* fp = fopen(fname.c_str(), "rb");
	if (fp == NULL) {
		char* tmp = new char[64];
		sprintf(tmp, "Cannot open \"%s\"!", fname.c_str());
		msgq.push(tmp);
		return;
	}

	fprintf(fp,"%s",root.c_str());
	fwrite(&num, sizeof(uint), 1, fp);

	for (auto it = file.begin(); it != file.end(); it++) {
		fprintf(fp,"%s",it->path.c_str());
		fwrite(&it->size, sizeof(uint), 1, fp);
		fwrite(&it->data,1,it->size,fp);

		char tmp[1024];
		sprintf(tmp, "Progress: %lld%% -- Wrote %s", (((100 / num) * (it - file.begin() + 1) )>>1) + 50,it->path.c_str());
		msgq.push(tmp);
	}

	fclose(fp);
	return;
}

void __DataBase__::Backup() {
	vector<string> fn=GetFilesUnderAPath(db.root);
	num = fn.size();
	for (auto it = fn.begin(); it != fn.end(); it++) {
		file.push_back(__FileData__());
		file.back().path = *it;

		struct stat statbuf;
		stat(it->c_str(), &statbuf);
		file.back().size = statbuf.st_size;

		file.back().Allocate();
		file.back().Read();

		if (file.back().err != 0)
			file.back().Release();
		
		char tmp[1024];
		sprintf(tmp,"Progress: %lld%% -- Read %s",100/num*(it-fn.begin()+1)>>1,it->c_str());
		msgq.push(tmp);
	}
	Write();
	msgq.push("Done");
	msgq.push("[end]");
	return;
}

void __DataBase__::Restore() {
	Read();
	for (auto it = file.begin(); it != file.end(); it++) {
		it->Write();
		it->Release();
	}
	msgq.push("Done");
	msgq.push("[end]");
	return;
}