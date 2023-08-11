#include "Dbr.h"

string GetExtension(string path) {
	int size = 1;
	for (int i = path.size() - 1; i >= 0; i--, size++)
		if (path[i] == '.')
			return path.substr(i, size);
		else if (path[i] == '\\')
			return string();
	return string();
}

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

vector<string> GetFilesUnderAPath(string path){
	vector<string> files;
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){
		do{
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0){
					vector<string> v=GetFilesUnderAPath(p.assign(path).append("\\").append(fileinfo.name));
					while (!v.empty()) {
						files.push_back(v.front());
						v.erase(v.begin());
					}
				}
			}
			else{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return files;
}

void AnalArgs(int argc, char** argv) {
	action = ACT_PRINTHELP;
	bool haspath=false, hasfile=false;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "/?")) {
			action = ACT_PRINTHELP;
			return;
		}
		else if (!strcmp(argv[i], "/b")) {
			if (action == ACT_RESTORE)
				puts("Warning: There are both /b and /r! Using /b as default.");
			action = ACT_BACKUP;
		}
		else if (!strcmp(argv[i], "/r")) {
			if (action == ACT_RESTORE)
				puts("Warning: There are both /b and /r! Using /r as default.");
			action = ACT_BACKUP;
		}
		else {
			struct _stat tmp;
			int rtn = _stat(argv[i],&tmp);
			FILE *fp = fopen(argv[i],"w");
			if (tmp.st_mode & S_IFDIR) {
				if(haspath)
					printf("Warning: There are more than one paths, using the last one as default.\n");
				db.root = argv[i];
				haspath = true;
			}
			else if ((tmp.st_mode & S_IFREG) || fp) {
				if (hasfile)
					printf("Warning: There are more than one files, using the last one as default.\n");
				db.fname = argv[i];
				hasfile = true;
			}
			else
				printf("Warning: Unknown argument \"%s\", ignored as default.\n", argv[i]);
			if (fp)
				fclose(fp);
		}
	}
	if (action == ACT_BACKUP) {
		if ((!haspath) || (!hasfile)) {
			puts("Error: Arguments don\'t have both [file] and [path]!");
			exit(1);
		}
		if (GetExtension(db.fname).empty())
			db.fname += ".dbr";
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