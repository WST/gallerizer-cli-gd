
#ifndef IMAGE_H
#define IMAGE_H

#include <gd.h>
#include <string>
#include <stdint.h>

class Image
{
	public:
		Image(std::string filename);
		Image(const Image &image);
		~Image();

		void resize(uint32_t width, uint32_t height);

	private:
		gdImagePtr image;
};

#endif
