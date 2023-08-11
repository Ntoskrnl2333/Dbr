#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <sys/stat.h>
#include <windows.h>
#include <cstring>
#include <conio.h>
#include <thread>
#include <string>
#include <cstdio>
#include <vector>
#include <queue>
#include <io.h>
using namespace std;

typedef unsigned int uint;

extern uint action;

enum ACTIONS {
	ACT_NONE,
	ACT_BACKUP,
	ACT_RESTORE,
	ACT_PRINTHELP,
};

extern queue<string> msgq;

extern string GetExtension(string path);
extern string SpecSlash(string path,bool isrelatve);
extern vector<string> GetFilesUnderAPath(string path);

class __FileData__ {
public:
	void Allocate();
	void Release();
	void Read(); //Read from file
	void Write(); //Write to file

	string path;
	uint size=0,err=0;
	char *data=NULL;
};

class __DataBase__ {
private:
	void Read(); //Read from .dbr
	void Write(); //Write to .dbr
public:
	void Backup();
	void Restore();

	string root,fname;
	uint num=0,nowfile=0;//nowfile:useless
	vector<__FileData__> file;
};

extern __DataBase__ db;

extern void AnalArgs(int argc, char** argv);
extern void Act();
extern void Backup();
extern void Restore();
extern void PrintHelp();
extern int main(int argc, char** argv);
