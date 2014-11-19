
#include "image.h"
#include "console.h"

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pcre.h>
#include <list>
#include <libconfig.h++>

bool fileExists(std::string filename) {
	return (access(filename.c_str(), F_OK) != -1);
}

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
	int slide_width, slide_height, thumbnail_width, thumbnail_height, slide_quality, thumbnail_quality;
  	try {
		slide_width = cfg.lookup("gallerizer.slides.width");
		slide_height = cfg.lookup("gallerizer.slides.height");
		thumbnail_width = cfg.lookup("gallerizer.thumbnails.width");
		thumbnail_height = cfg.lookup("gallerizer.thumbnails.height");
		slide_quality = cfg.lookup("gallerizer.slides.quality");
		thumbnail_quality = cfg.lookup("gallerizer.thumbnails.quality");
	}
	catch(const libconfig::SettingNotFoundException &nfex) {
		return fatal(" Необходимые параметры в конфигурационном файле неверны или отсутствуют", -1);
	}
	catch(const libconfig::SettingTypeException &stex) {
		return fatal(" Необходимые параметры в конфигурационном файле неверны или отсутствуют", -1);
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

	sprintf(buf, " Найдено %ld изображений\n Целевой размер слайдов: %d×%d, эскизов: %d×%d\n Качество эскизов: %d, слайдов: %d", files.size(), slide_width, slide_height, thumbnail_width, thumbnail_height, thumbnail_quality, slide_quality);
	information(buf, false, true);

	struct stat st = {0};
	if(stat("slides", &st) == -1) {
		mkdir("slides", 0755);
	}
	if(stat("thumbnails", &st) == -1) {
		mkdir("thumbnails", 0755);
	}

	int current = 0;
	for(filelist::iterator item = files.begin(); item != files.end(); ++ item) {

		// Проверка
		std::string slide_filename = std::string("slides/") + *item;
		if(fileExists(slide_filename)) {
			Image *slide = new Image(slide_filename);
			if((slide->width() != slide_width) || slide->height() != slide_height) {
				//existsButWrong(++ current, files.size(), *item);
			} else {
				alreadyExists(++ current, files.size(), *item);
				continue;
			}
			delete slide;
		}

		Image *image = new Image(*item);
		processed(++ current, files.size(), *item, image->width(), image->height());

		// Слайд
		image->resize(slide_width, slide_height);
		image->saveAs(slide_filename, slide_quality);

		// Эскиз
		image->resize(thumbnail_width, thumbnail_height);
		image->saveAs(std::string("thumbnails/") + *item, thumbnail_quality);

		delete image;
	}

	information(" Задание успешно выполнено", false, true);

	return 0;
}
