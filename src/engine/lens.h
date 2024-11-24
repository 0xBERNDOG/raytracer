#ifndef LENS_H
#define LENS_H

#include "engine/vector.h"

struct ray;

struct lens {
	struct vector position;
	struct vector normal;

	double r1;
	double r2;
	double separation;
};

struct object create_lens(struct lens *lens);
optional_vector lens_ray_intersection(void *lens, struct ray ray);
optional_vector lens_hit_normal(void *lens, struct vector hit_position);

bool lens_ray_entering(void *lens, struct ray ray);
bool lens_ray_leaving(void *lens, struct ray ray);

#endif