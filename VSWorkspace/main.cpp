#include "Dbr.h"

unsigned int action;
queue<string> msgq;
__DataBase__ db;

int main(int argc, char** argv) {
	AnalArgs(argc,argv);
	Act();
	return 0;
}