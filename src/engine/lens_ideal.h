#ifndef LENS_IDEAL_H
#define LENS_IDEAL_H

#include "engine/ray.h"
#include "engine/vector.h"

struct ray;

struct lens_ideal {
	struct vector position;
	struct vector normal;

	double r;
	double f;
};

struct object create_lens_ideal(struct lens_ideal *lens);
optional_vector lens_ideal_ray_intersection(void *lens, struct ray ray);
optional_vector lens_ideal_hit_normal(void *lens, struct vector hit_position);
optional_ray lens_ideal_refraction(struct ray, struct hit_data);

bool lens_ideal_ray_entering(void *lens, struct ray ray);
bool lens_ideal_ray_leaving(void *lens, struct ray ray);

#endif