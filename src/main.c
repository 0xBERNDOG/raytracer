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
		           .normal = { 0.0, 0.0, 1.0 } },
		.width = 10,
		.height = 10,
		.pixel_spacing = 1.0
	};

	struct sphere sphere = { .position = { .x = 5.0, .y = 5.0, .z = 5.0 },
		                 .radius = 1 };
	struct object sphere_obj = create_sphere(&sphere);
	world[0] = sphere_obj;

	struct image *image = sensor_capture(params, world, 1);

	// todo: get filename from input to main()
	file_write();

	// todo:
	// define scene
	//      objects, with refraction, lenses
	// spawn + shoot rays

	free(world);
}
