
#include "console.h"

void information(std::string text, bool newline, bool wrap) {
	if(wrap) std::cout << std::endl;
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << text;
	return;
#else
	std::cout << "\033[22;37m" << text << "\033[0m" << std::endl;
#endif
	if(newline) std::cout << std::endl;
	if(wrap) std::cout << std::endl;
}

int fatal(std::string text, int exitcode) {
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cerr << text << std::endl;
	if(exitcode != 0) {
		Sleep(5000); // Windows users usually run applications by mouseclick
		exit(exitcode);
	}
	return exitcode;
#else
	std::cerr << "\033[22;31m" << text << "\033[0m" << std::endl;
	return exitcode;
#endif
}

void processed(int current, long total, std::string filename, int width, int height) {

	std::cout << "\033[01;35m" << " [" << current << "/" << total << "] " << "\033[0m";

	std::cout << "Сжимаем: ";

	std::cout << "\033[01;33m" << filename << "\033[0m";


	std::cout << " <";
		std::cout << width;
		std::cout << "×";
		std::cout << height;
	std::cout << ">";

	std::cout << std::endl;
}

void alreadyExists(int current, long total, std::string filename) {
	std::cout << "\033[01;35m" << " [" << current << "/" << total << "] " << "\033[0m";
	std::cout << "Изображение уже обработано ранее: ";
	std::cout << "\033[01;33m" << filename << "\033[0m";
	std::cout << std::endl;
}
