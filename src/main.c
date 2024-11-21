#include <stdio.h>
#include <stdlib.h>

#include "engine/image.h"
#include "engine/object.h"
#include "engine/ray.h"
#include "engine/sensor.h"
#include "engine/sphere.h"
#include "utils/file.h"
#include "version.h"

// todo: something's wrong with the refraction

int
main()
{
	// todo: populate the world
	struct object *world = calloc(10, sizeof(struct object));
	// todo: handle if calloc fails

	double f = 24.0;
	struct sensor_params params = { .plane = { .position = { -f, 0.0, 0.0 },
		                                   .normal = { 1.0, 0.0,
		                                               0.0 } },
		                        .width = 1000,
		                        .height = 1000,
		                        .pixel_spacing = 0.01 };

	struct sphere sphere = { .position = { f, 0.0, 0.0 }, .radius = 1.25 };
	struct object sphere_obj = create_sphere(&sphere);
	// todo: improve how lighting data is passed to object?
	sphere_obj.brightness = 0.7;
	sphere_obj.reflectivity = CREATE_OPTIONAL(double, 0.25);
	world[0] = sphere_obj;

	struct sphere sphere2 = { .position = { f - 0.5, 0.5, 0.9 },
		                  .radius = 0.75 };
	struct object sphere_obj2 = create_sphere(&sphere2);
	sphere_obj2.brightness = 0.0;
	// sphere_obj2.reflectivity = CREATE_OPTIONAL(double, 0.4);
	sphere_obj2.refractive_index = CREATE_OPTIONAL(double, 1.1);
	sphere_obj2.transmissivity = CREATE_OPTIONAL(double, 0.4);
	world[1] = sphere_obj2;

	struct sphere sphere3 = { .position = { f - 0.25, 0.5, 1.5 },
		                  .radius = 0.25 };
	struct object sphere_obj3 = create_sphere(&sphere3);
	sphere_obj3.brightness = 0.9;
	world[2] = sphere_obj3;

	struct sphere sphere4 = { .position = { 0.0, 0.0, 0.0 },
		                  .radius = 10.0 };
	struct object sphere_obj4 = create_sphere(&sphere4);
	sphere_obj4.brightness = 0.0;
	sphere_obj4.refractive_index = CREATE_OPTIONAL(double, 1.5);
	sphere_obj4.transmissivity = CREATE_OPTIONAL(double, 0.99);
	world[3] = sphere_obj4;

	struct image *image = sensor_capture(params, world, 4);

	// todo: get filename from input to main()
	file_write(image);

	// todo:
	// define scene
	//      objects, with refraction, lenses
	// spawn + shoot rays

	free(world);
	image_free(image);
}
