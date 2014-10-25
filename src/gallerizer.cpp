
#include "image.h"
#include "console.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pcre.h>
#include <list>

typedef std::list<std::string> filelist;


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

	filelist files;

	// Откроем каталог и обойдём его в цикле
	DIR *directory = opendir(cwd);
	struct dirent *item;
	while((item = readdir(directory)) != 0) {
		if(pcre_exec(re, 0, item->d_name, strlen(item->d_name), 0, 0, ovector, 30) == -1) continue;
		files.push_back(std::string(item->d_name));
	}
	closedir(directory);

	// Для сообщений в консоль
	char buf[100];

	int count = files.size();
	if(count == 0) {
		return fatal(" Изображения не найдены, обрабатывать нечего", -1);
	}

	sprintf(buf, " Найдено %ld изображений", files.size());
	information(buf);

	for(filelist::iterator item = files.begin(); item != files.end(); ++ item) {
		sprintf(buf, " Обрабатывается изображение: %s", item->c_str());
		information(buf);

		Image *image = new Image(*item);
		image->resize(640, 480);
		delete image;
	}

	return 0;
}
