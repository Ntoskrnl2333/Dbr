#include "Dbr.h"

void PrintHelp() {
	puts("Backup or restore your files.");
	puts("");
	puts("DBR.EXE [/b /r] [path] [file]");
	puts("");
	puts("     /b    Backup the files in [path] into [file].");
	puts("           Cannot with /r.");
	puts("");
	puts("     /r    Restore the files in [file] to [path],");
	puts("           if there\'s no [path], the files will restore to where they are in.");
	puts("           Cannot with /b.");
	puts("");
	puts("     path  If with /b, it\'s the path where it will backup.");
	puts("           If with /r, it\'s the path where it will restore.");
	puts("");
	puts("     file  If with /b, it\'s the file which it will write.");
	puts("           If with /r, it\'s the file which it will read.");
	puts("           If the name doesn\'t have extension when with /b, it will add a \".dbr\" as default.");
	puts("");
	return;
}