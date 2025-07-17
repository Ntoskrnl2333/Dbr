#include "dbr.h"
using namespace std;

std::string folderpath, filepath;
FILE *fdbr;
bool started, working, filecopied_lock = LOCK_RELEASE;
unsigned long long int bytescopied;
std::queue<string> filecopied;

int main(int argc, char **argv){
	std::thread *th;
	switch (AnalArgs(argc, argv)) {
	case ACT_PRINT_HELP:
		PrintHelp();
		break;
	case ACT_BACKUP:
		th = new std::thread(BackupWorker);
		MessageWorker();
		break;
	case ACT_RESTORE:
		th = new std::thread(RestoreWorker);
		MessageWorker();
		break;
	}
	return 0;
}

ACTION AnalArgs(int argc, char** argv) {
	bool backup=false, restore=false;
	string path, file;

	for (int i = 1; i < argc; i++)
		if (!_stricmp("-b", argv[i])) {
			backup = true;
			if (restore)
				return ACT_PRINT_HELP;
		}
		else if (!_stricmp("-r", argv[i])) {
			restore = true;
			if (backup)
				return ACT_PRINT_HELP;
		}
		else {
			std::cout << argv[i] << '\n';
			string s = std::filesystem::absolute(argv[i]).string();
			if (s.back() == '\\') {
				if (!path.empty())
					return ACT_PRINT_HELP;
				path = s;
			}
			else {
				if (!file.empty())
					return ACT_PRINT_HELP;
				file = s;
			}
		}

	folderpath = path;
	filepath = file;

	if (filepath.empty())
		return ACT_PRINT_HELP;

	if (backup) {
		std::filesystem::path pa(path);
		if (!std::filesystem::is_directory(pa))
			return ACT_PRINT_HELP;
	}
	else if (restore) {
		std::filesystem::path pa(file);
		if (!std::filesystem::is_regular_file(pa))
			return ACT_PRINT_HELP;
	}
	else
		return ACT_PRINT_HELP;

	return backup ? ACT_BACKUP : ACT_RESTORE;
}
