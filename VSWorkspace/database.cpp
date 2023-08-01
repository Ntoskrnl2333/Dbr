#include "Dbr.h"

uint __DataBase__::Read(){
	FILE* fp = fopen(fname.c_str(), "rb");
	if (fp == NULL) {
		char *tmp=new char[64];
		sprintf(tmp,"Cannot open \"%s\"!",fname.c_str());
		msgq.push(tmp);
	}

	char tmps[1024];
	memset(tmps, 0, sizeof(tmps));

	if (fscanf(fp, "%s", tmps) != 1) {
		msgq.push("File format error!");
		fclose(fp);
		return 1;
	}
	root = tmps;
	

	if (fread(&num, sizeof(uint), 1, fp)!=sizeof(uint) ){
		msgq.push("File format error!");
		fclose(fp);
		return 1;
	}

	for (uint i = 0; i <= num; i++) {
		__FileData__ fd;

		memset(tmps, 0, sizeof(tmps));
		if (fscanf(fp, "%s", tmps) != 1) {
			msgq.push("File format error!");
			fclose(fp);
			return 1;
		}
		
		fd.path = root+tmps;
		if (fread(&fd.size, sizeof(uint), 1, fp) != sizeof(uint)) {
			msgq.push("File format error!");
			fclose(fp);
			return 1;
		}
		fd.Allocate();
		fd.Read();
		if (fd.err != 0) {
			fd.Release();
			fclose(fp);
			return 1;
		}
		file.push_back(fd);
	}

	fclose(fp);
	return 0;
}

void __DataBase__::Write(){

	return;
}

void __DataBase__::Backup() {
	
	return;
}

void __DataBase__::Restore() {
	return;
}