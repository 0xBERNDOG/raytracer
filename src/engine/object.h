#ifndef OBJECT_H
#define OBJECT_H

#include "engine/vector.h"

struct ray;

struct object {
	void *object;    // "this" (pointer to object data, e.g. struct sphere)

	optional_vector (*func_ray_intersection)(void *, struct ray *);

	// todo: replace with functions
	// todo: upgrade from B+W to colour
	double reflectivity;
	double brightness;
};

#endif
