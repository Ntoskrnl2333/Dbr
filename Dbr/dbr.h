#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <queue>
#include <thread>
#include <chrono>
#include <format>
#include <cstdlib>
#include <windows.h>
#include <algorithm>

#ifdef _DEBUG
#define DEBUG
#endif

enum ACTION{
	ACT_PRINT_HELP,
	ACT_BACKUP,
	ACT_RESTORE,
};

enum {
	LOCK_RELEASE,
	LOCK_BUSY,
};

enum RESTORE_STATE {
	RS_ROOT,
	RS_NAME,
	RS_DATA,
};

inline constexpr int BUFFER_SIZE = 1048576;

extern ACTION AnalArgs(int argc, char** argv);
extern void PrintHelp();

extern std::string folderpath, filepath;
extern FILE *fdbr;

extern bool started, working, filecopied_lock;
extern unsigned long long int bytescopied;
extern std::queue<std::string> filecopied;

extern void BackupWorker();
extern void RestoreWorker();

extern void EncodeString(char* s, unsigned int len, char* out);
extern void EncodeString(char* s, unsigned int len, char* out);

extern std::string ByteToReadable(unsigned long long int size);
extern void MessageWorker();