
#include "console.h"

void information(std::string text) {
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << text << std::endl;
	return;
#endif
	std::cout << "\033[22;37m" << text << "\033[0m" << std::endl;
}

int fatal(std::string text, int exitcode) {
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cout << text << std::endl;
	if(exitcode != 0) {
		Sleep(5000); // Windows users usually run applications by mouseclick
		exit(exitcode);
	}
	return;
#endif
	std::cout << "\033[22;31m" << text << "\033[0m" << std::endl;
	return exitcode;
}
