#include "Dbr.h"

string SpecSlash(string path, bool isr){
	for(int i=0;i<path.size();i++)
		if(path[i]=='/')
			path[i]='\\';
	if (isr && (path.front() != '\\'))
		path='\\'+path;
	else if((!isr) && (path.back()=='\\'))
		path=path.substr(1);
	return path;
}

void AnalArgs(int argc, char** argv) {
	action = ACT_PRINTHELP;
	bool haspath=false, hasfile=false;
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
				if(haspath)
					printf("Warning: There are more than one paths, using the last one as default.\n");
				db.root = argv[i];
				haspath = true;
			}
			else if (tmp.st_mode & S_IFREG) {
				if (hasfile)
					printf("Warning: There are more than one files, using the last one as default.\n");
				db.fname = argv[i];
				hasfile = true;
			}
			else
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
			exit(1);
		}
		if (!haspath) {
			char buf[1024];
			_fullpath(buf, db.fname.c_str(), 1024);
			db.root = buf;
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