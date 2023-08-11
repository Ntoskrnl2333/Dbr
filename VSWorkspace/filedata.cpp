#include "Dbr.h"

void __FileData__::Allocate() {
	Release();
	if (size > 0)
		data = new char[size];
	return;
}

void __FileData__::Release(){
	if (data != NULL)
		delete data;
	return;
}

void __FileData__::Read() {
	FILE* fp = fopen(path.c_str(), "rb");
	if (fp == NULL) {
		char *tmp=new char[64];
		sprintf(tmp, "Warning: Cannot read \"%s\", ignored.", path.c_str());
		msgq.push(tmp);
		err = 1;
		return;
	}
	memset(data,0,sizeof(char)*size);
	uint rtn = fread(data, 1, size, fp);
	if (rtn != size){
		msgq.push("File format error!");
		err = 2;
	}
	fclose(fp);
	return;
}

void __FileData__::Write() {
	FILE* fp = fopen(path.c_str(), "wb");
	if (fp == NULL) {
		char tmp[1024];
		sprintf(tmp, "Warning: Cannot write \"%s\", ignored.", path.c_str());
		msgq.push(tmp);
		err = 1;
		return;
	}
	fwrite(data, 1, size, fp);
	fclose(fp);
	return;
}