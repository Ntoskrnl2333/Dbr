#include "dbr.h"

void PrintHelp() {
	puts("Backup or restore your files through English letters.");
	puts("");
	puts("dbr.exe (-b | -r) <path> <file>");
	puts("");
	puts("    -b    Backup the files in <path> into <file>.");
	puts("          Cannot with -r.");
	puts("");
	puts("    -r    Restore the files in <file> to <path>,");
	puts("          if there\'s no <path>, the files will restore to where they were in.");
	puts("          Cannot with -b.");
	puts("");
	puts("    path  If with -b, it\'s the path where it will backup.");
	puts("          If with -r, it\'s the path where it will restore.");
	puts("          Must end with \"\\\", or it\'ll be considered a file name.");
	puts("");
	puts("    file  If with -b, it\'s the file which it will write.");
	puts("          If with -r, it\'s the file which it will read.");
	puts("          If the name doesn\'t have extension when with -b, it will (NOT, fixme) add a \".dbr\" as default.");
	puts("");
	puts("Before taking action, you should leave enough disk space.");
	puts("  Insufficient space will cause uncontrollable consequences.");
	puts("");
	return;
}