#include "utils/file.h"
#include "engine/image.h"
#include "engine/vector.h"
#include "libbmp/libbmp.h"

#include <stddef.h>

int
file_write(struct image *image)
{
	bmp_img img;
	bmp_img_init_df(&img, image->width, image->height);

	for (size_t x = 0; x < image->width; x++) {
		for (size_t y = 0; y < image->height; y++) {
			size_t i = x * image->width + y;
			struct vector pixel = image->pixels[i];

			bmp_pixel_init(&img.img_pixels[y][x], pixel.x, pixel.y,
			               pixel.z);
		}
	}

	// todo: replace with actual file name input
	bmp_img_write(&img, "test.bmp");
	bmp_img_free(&img);
	return 0;
}