#ifndef HIT_DATA_H
#define HIT_DATA_H

#include "engine/vector.h"

struct object;

struct hit_data {
	struct object *object;

	struct vector position;
	struct vector normal;

	// todo: surface params (reflectivity, transmissivity etc,
	// roughness/angle spread around ray directions)

	struct vector colour;
};

#endif