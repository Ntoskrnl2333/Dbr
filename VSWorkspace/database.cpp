#include "Dbr.h"

void __DataBase__::Read(){
	return;
}

void __DataBase__::Write(){
	return;
}

void __DataBase__::Backup() {
	for (int i = 0; i < file.size(); i++)
		file[i].Read();
	return;
}

void __DataBase__::Restore() {
	return;
}