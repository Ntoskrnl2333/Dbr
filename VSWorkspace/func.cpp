#include "Dbr.h"

void AnalArgs(int argc, char** argv) {
	action = ACT_PRINTHELP;
	bool haspath=false, hasfile=false;
	string argfile;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "/?")) {
			action = ACT_PRINTHELP;
			return;
		}
		else if (strcmp(argv[i], "/b")) {
			if (action == ACT_RESTORE)
				puts("Warning: There are both /b and /r! Using /b as default.");
			action = ACT_BACKUP;
		}
		else if (strcmp(argv[i], "/r")) {
			if (action == ACT_RESTORE)
				puts("Warning: There are both /b and /r! Using /r as default.");
			action = ACT_BACKUP;
		}
		else {
			struct _stat tmp;
			int rtn = _stat(argv[i],&tmp);
			if (tmp.st_mode & S_IFDIR) {
				db.root = argv[i];
				haspath = true;
			}
			else if (tmp.st_mode & S_IFREG) {
				argfile = argv[i];
				hasfile = true;
			}
			printf("Warning: Unknown argument \"%s\", ignored as default.\n", argv[i]);
			return;
		}
	}
	if (action == ACT_BACKUP) {
		if ((!haspath) || (!hasfile)) {
			puts("Error: Arguments don\'t have both [file] and [path]!");
			exit(1);
		}
	}
	else if (action == ACT_RESTORE) {
		if (!hasfile) {
			puts("Error: Arguments don\'t have [file]!");
		}
	}
	return;
}

void Act() {
	if (action == ACT_PRINTHELP)
		PrintHelp();
	else if (action == ACT_BACKUP)
		Backup();
	else if (action == ACT_RESTORE)
		Restore();
	else {
		puts("Unknown error!");
		exit(1);
	}
	return;
}