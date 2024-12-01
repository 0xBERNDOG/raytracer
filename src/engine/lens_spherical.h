#ifndef LENS_SPHERICAL_H
#define LENS_SPHERICAL_H

#include "engine/vector.h"

// todo: replace with ideal lens with a custom func_ray_refraction

struct ray;

struct lens_spherical {
	struct vector position;
	struct vector normal;

	double r1;
	double r2;
	double separation;    // todo: replace with lens radius?
};

struct object create_lens_spherical(struct lens_spherical *lens);
optional_vector lens_spherical_ray_intersection(void *lens, struct ray ray);
optional_vector lens_spherical_hit_normal(void *lens,
                                          struct vector hit_position);

bool lens_spherical_ray_entering(void *lens, struct ray ray);
bool lens_spherical_ray_leaving(void *lens, struct ray ray);

#endif