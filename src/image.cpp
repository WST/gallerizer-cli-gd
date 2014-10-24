
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

void Image::resize(uint32_t width, uint32_t height) {
	gdImagePtr old = image;
	image = gdImageCreateTrueColor(width, height);
	gdImageCopyResampled(image, old, 0, 0, 0, 0, width, height, gdImageSX(old), gdImageSY(old));
	gdImageDestroy(old);
}
