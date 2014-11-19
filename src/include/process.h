#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <stdint.h>

#include <libconfig.h++>

class Process
{
	public:
		Process(std::string path);
		~Process();
		void run();
		void loadConfiguration(std::string filename);

	private:
		bool slideExists(std::string filename);
		bool thumbnailExists(std::string filename);
		void renderThumbnail(std::string filename);
		void renderSlide(std::string filename);

		libconfig::Config cfg;
		std::string directory;
};

#endif
