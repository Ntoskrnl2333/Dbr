#include "dbr.h"

#include <string>
#include <cmath>

std::string ByteToReadable(unsigned long long int _size_) {
	std::string units[] = { "B", "KB", "MB", "GB", "TB", "PB" };
	int unitIndex = 0;
	double size = _size_;

	if (size == 0)
		return "0 B";

	while (size > 1024 && unitIndex < 5) {
		size /= 1024;
		++unitIndex;
	}

	return std::to_string(size) + ' ' + units[unitIndex];
}

void MessageWorker() {
#define GETTIMEMS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
	while (!started);
	unsigned long long int lastbyte=0, filecnt=0, lastms=GETTIMEMS, deltabyte=0, deltams=0;
	unsigned int overridelen = 0;
	working = true;
	while (working) {
		if (!filecopied_lock) {
			filecopied_lock = true;
			while (!filecopied.empty()) {
				printf("%s", filecopied.front().c_str());
				if (filecopied.front().size() < overridelen) {
					for (int i = 0; i < overridelen - filecopied.front().size() + 4; i++)
						putchar(' ');
					overridelen = 0;
				}
				putchar('\n');
				filecnt++;
				filecopied.pop();
			}
			filecopied_lock = false;
		}

		if (GETTIMEMS - lastms > 200){
			deltabyte = bytescopied - lastbyte;
			deltams = GETTIMEMS - lastms;
			lastms = GETTIMEMS;
			lastbyte = bytescopied;
		}
		if (deltams == 0)
			deltams = 1;

		std::string buf = format("    === File processed: {}    Process speed: {}/s ===",
								filecnt,
								ByteToReadable(deltabyte / deltams * 1000));
		overridelen = buf.size();
		
		printf("%s",buf.c_str());
		for (int i = 0; i < buf.size(); i++)
			putchar('\b');
	}
}