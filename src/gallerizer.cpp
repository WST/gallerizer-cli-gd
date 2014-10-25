
#include "image.h"
#include "console.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pcre.h>
#include <list>
#include <libconfig.h++>

typedef std::list<std::string> filelist;


int main(int argc, char *argv[]) {
	// Для сообщений в консоль
	char buf[100];

	information(" Gallerizer — утилита для создания галерей\n Консольная версия\n (c) 2014 Илья Аверков <ilya@averkov.net>", false, true);

	// Конфигурационный файл
	libconfig::Config cfg;
	try {
		cfg.readFile("/etc/gallerizer.cfg");
	}
	catch(const libconfig::FileIOException &fioex) {
		return fatal(" Ошибка чтения /etc/gallerizer.cfg", -1);
	}
	catch(const libconfig::ParseException &pex)
	{
		sprintf(buf, " Ошибка чтения %s: строка %d — %s", pex.getFile(), pex.getLine(), pex.getError());
		return fatal(buf, -1);
	}

	// Запросим переменные из конфигурационного файла
	int width, height;
  	try {
		width = cfg.lookup("gallerizer.width");
		height = cfg.lookup("gallerizer.height");
	} catch(const libconfig::SettingNotFoundException &nfex) {
		return fatal(" Необходимые параметры width и height в конфигурационном файле неверны", -1);
	}

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

	int count = files.size();
	if(count == 0) {
		return fatal(" Изображения не найдены, обрабатывать нечего", -1);
	}

	sprintf(buf, " Найдено %ld изображений\n Целевой размер слайдов: %d×%d", files.size(), width, height);
	information(buf, false, true);

	int current = 0;
	for(filelist::iterator item = files.begin(); item != files.end(); ++ item) {
		Image *image = new Image(*item);

		sprintf(buf, "[%d/%ld] Сжимаем: %s <%d×%d>", ++ current, files.size(), item->c_str(), image->width(), image->height());
		information(buf);

		image->resize(width, height);
		delete image;
	}

	information(" Задание успешно выполнено", false, true);

	return 0;
}
