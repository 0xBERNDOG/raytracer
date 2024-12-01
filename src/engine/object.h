#ifndef OBJECT_H
#define OBJECT_H

#include "engine/ray.h"
#include "engine/vector.h"

struct hit_data;

struct object {
	void *object;    // "this" (pointer to object data, e.g. struct sphere)

	optional_vector (*func_ray_intersection)(void *, struct ray);
	optional_ray (*func_ray_refraction)(struct ray, struct hit_data);
	optional_vector (*func_hit_normal)(void *, struct vector);
	bool (*func_ray_entering)(void *, struct ray);
	bool (*func_ray_leaving)(void *, struct ray);

	// todo: replace with functions
	// todo: upgrade from B+W to colour
	double brightness;
	optional_double reflectivity;
	optional_double transmissivity;
	optional_double refractive_index;
};

optional_ray object_default_refraction(struct ray, struct hit_data);

#endif
