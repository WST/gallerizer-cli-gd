
#ifndef IMAGE_H
#define IMAGE_H

#include <gd.h>
#include <string>

class Image
{
	public:
		Image(std::string filename);
		Image(const Image &image);
		~Image();

	private:
		gdImagePtr image;
};

#endif
