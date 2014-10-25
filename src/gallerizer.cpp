
#include "image.h"
#include "console.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pcre.h>
#include <list>


int main(int argc, char *argv[]) {
	// Сначала скомпилируем регулярное выражение для проверки имён файлов
	int options = 0;
	const char *error;
	int erroffset;
	pcre *re = pcre_compile("(jpe?g|JPE?G)$", options, &error, &erroffset, NULL);

	// Массив для соответствий
	int ovector[30];

	// Запросим текущий активный каталог
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	std::list<std::string> files;

	// Откроем каталог и обойдём его в цикле
	DIR *directory = opendir(cwd);
	struct dirent *item;
	while((item = readdir(directory)) != 0) {
		if(pcre_exec(re, 0, item->d_name, strlen(item->d_name), 0, 0, ovector, 30) == -1) continue;
		files.push_back(std::string(item->d_name));
	}
	closedir(directory);

	int count = files.size();
	if(count == 0) {
		return fatal(" Изображения не найдены, обрабатывать нечего", -1);
	}

	char buf[100];
	sprintf(buf, " Найдено %ld изображений", files.size());
	information(buf);


	return 0;
}
