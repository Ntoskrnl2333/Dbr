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
	FILE* fp = fopen(path.c_str(),"r");
	if (fp == NULL) {
		char tmp[1024];
		sprintf(tmp,"Warning: Cannot read \"%s\", ignored.",path.c_str());
		msgq.push(tmp);
		return;
	}
	fread(data,1,size,fp);
	fclose(fp);
	return;
}

void __FileData__::Write() {
	FILE* fp = fopen(path.c_str(), "w");
	if (fp == NULL) {
		char tmp[1024];
		sprintf(tmp, "Warning: Cannot write \"%s\", ignored.", path.c_str());
		msgq.push(tmp);
		return;
	}
	fwrite(data, 1, size, fp);
	fclose(fp);
	return;
}