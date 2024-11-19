#ifndef HIT_DATA_H
#define HIT_DATA_H

#include "engine/vector.h"
#include "utils/optional.h"

struct object;

struct hit_data {
	struct object *object;

	struct vector position;
	struct vector normal;

	// todo: surface params (reflectivity, transmissivity etc,
	// roughness/angle spread around ray directions)

	double brightness;    // starting with black+white only
	optional_double reflectivity;
	optional_double transmissivity;
	optional_double refractive_index;
};

typedef optional_type(struct hit_data) optional_hit_data;

#endif