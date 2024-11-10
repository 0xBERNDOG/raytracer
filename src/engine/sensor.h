#ifndef SENSOR_H
#define SENSOR_H

#include "engine/object.h"
#include "engine/plane.h"

#include <stddef.h>

struct sensor_params {
	struct plane plane;

	// pixels
	size_t width;
	size_t height;

	double pixel_spacing;    // for now, pixels are single points (not
	                         // squares) with a defined spacing
};

struct vector *sensor_capture(struct sensor_params, struct object objects[],
                              size_t num_objects);

#endif
