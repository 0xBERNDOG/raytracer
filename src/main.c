#include <stdio.h>
#include <stdlib.h>

#include "engine/image.h"
#include "engine/object.h"
#include "engine/ray.h"
#include "engine/sensor.h"
#include "engine/sphere.h"
#include "utils/file.h"
#include "version.h"

int
main()
{
	// todo: populate the world
	struct object *world = calloc(10, sizeof(struct object));
	// todo: handle if calloc fails

	struct sensor_params params = {
		.plane = { .position = { 0.0, 0.0, 0.0 },
		           .normal = { 1.0, 0.0, 0.0 } },
		.width = 2000,
		.height = 2000,
		.pixel_spacing = 0.005
	};

	struct sphere sphere = { .position = { 5.0, 0.5, 0.5 }, .radius = 1 };
	struct object sphere_obj = create_sphere(&sphere);
	world[0] = sphere_obj;

	struct sphere sphere2 = { .position = { 2.5, 0.0, -0.5 },
		                  .radius = 0.5 };
	struct object sphere_obj2 = create_sphere(&sphere2);
	world[1] = sphere_obj2;

	struct image *image = sensor_capture(params, world, 2);

	// todo: get filename from input to main()
	file_write(image);

	// todo:
	// define scene
	//      objects, with refraction, lenses
	// spawn + shoot rays

	free(world);
	image_free(image);
}
