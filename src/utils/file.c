#include "utils/file.h"
#include "libbmp/libbmp.h"
#include <stddef.h>

int
file_write()
{
	bmp_img img;
	bmp_img_init_df(&img, 512, 512);    // todo: vary imagine size

	// todo: sample code, replace with reading sensor data
	for (size_t y = 0, x; y < 512; y++) {
		for (x = 0; x < 512; x++) {
			if ((y % 128 < 64 && x % 128 < 64) ||
			    (y % 128 >= 64 && x % 128 >= 64)) {
				bmp_pixel_init(&img.img_pixels[y][x], 250, 250,
				               250);
			} else {
				bmp_pixel_init(&img.img_pixels[y][x], 0, 0, 0);
			}
		}
	}

	// todo: replace with actual file name input
	bmp_img_write(&img, "test.bmp");
	bmp_img_free(&img);
	return 0;
}