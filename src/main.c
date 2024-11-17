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

	struct sphere sphere = { .position = { 5.0, -7.0, 0.0 },
		                 .radius = 5.0 };
	struct object sphere_obj = create_sphere(&sphere);
	// todo: improve how lighting data is passed to object?
	sphere_obj.brightness = 0.8;
	sphere_obj.reflectivity = 0.0;
	world[0] = sphere_obj;

	struct sphere sphere2 = { .position = { 5.0, 0.0, 0.0 },
		                  .radius = 1.0 };
	struct object sphere_obj2 = create_sphere(&sphere2);
	sphere_obj2.brightness = 0.0;
	sphere_obj2.reflectivity = 0.5;
	world[1] = sphere_obj2;

	struct plane plane = { .position = { 5.0, 1.0, 5.0 },
		               .normal = { 1.0, 1.0, 0.25 } };
	struct object plane_obj = create_plane(&plane);
	plane_obj.brightness = 0.0;
	plane_obj.reflectivity = 0.5;
	world[2] = plane_obj;

	struct image *image = sensor_capture(params, world, 3);

	// todo: get filename from input to main()
	file_write(image);

	// todo:
	// define scene
	//      objects, with refraction, lenses
	// spawn + shoot rays

	free(world);
	image_free(image);
}
