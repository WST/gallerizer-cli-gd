
#ifndef IMAGE_H
#define IMAGE_H

#include <gd.h>
#include <string>
#include <stdint.h>
#include <stdio.h>

class Image
{
	public:
		Image(std::string filename);
		Image(const Image &image);
		~Image();

		void resize(int width, int height);
		int width();
		int height();
		void saveAs(std::string filename, int quality = 90);

	private:
		gdImagePtr image;
};

#endif
