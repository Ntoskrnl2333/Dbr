#include "Dbr.h"

namespace front_cpp {
	void DB_BACKUP() {
		db.Backup();
		return;
	}
	void DB_RESTORE() {
		db.Restore();
		return;
	}
	void SHOW_MSG() {
		while (true) {
			while (msgq.empty());
			string get=msgq.front();
			msgq.pop();
			if (get == "[end]")
				break;
			printf("%s\n",get.c_str());
		}
		return;
	}
}

void Backup() {
	thread th(front_cpp::DB_BACKUP);
	front_cpp::SHOW_MSG();
	return;
}

void Restore() {
	thread th(front_cpp::DB_RESTORE);
	front_cpp::SHOW_MSG();
	return;
}