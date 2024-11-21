#ifndef RAY_H
#define RAY_H

#include <stddef.h>

#include "engine/hit_data.h"
#include "engine/object.h"
#include "engine/vector.h"

struct object;
struct sensor_params;

struct ray {
	struct vector position;
	struct vector direction;
};

optional_hit_data ray_get_hit_data(struct ray ray, struct object objects[],
                                   size_t num_objects, struct object *last_hit);

#endif
