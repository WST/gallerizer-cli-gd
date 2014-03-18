
#include "image.h"

Image::Image(std::string filename) {
	FILE *file = fopen(filename.c_str(), "r");
	image = gdImageCreateFromJpeg(file);
	fclose(file);
}

Image::Image(const Image &image) {
	
}

Image::~Image() {
	gdImageDestroy(image);
}
