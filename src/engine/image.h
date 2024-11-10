#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>

struct vector;

struct image {
	struct vector *pixels;

	size_t width;
	size_t height;
};

struct image *image_allocate(size_t width, size_t height);
void image_free(struct image *image);

#endif