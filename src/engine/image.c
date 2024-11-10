#include "engine/image.h"
#include "engine/vector.h"

#include <stdlib.h>

struct image *
image_allocate(size_t width, size_t height)
{
	struct image *image = calloc(1, sizeof(struct image));
	if (image == NULL) {
		return NULL;
	}

	struct vector *pixels = calloc(width * height, sizeof(struct vector));
	if (pixels == NULL) {
		free(image);
		return NULL;
	}

	image->pixels = pixels;
	return image;
}

void
image_free(struct image *image)
{
	if (image == NULL) {
		return;
	}

	if (image->pixels != NULL) {
		free(image->pixels);
	}

	free(image);
}