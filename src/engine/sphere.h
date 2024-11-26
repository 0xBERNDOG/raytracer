#ifndef SPHERE_H
#define SPHERE_H

#include "engine/vector.h"

#include <stddef.h>

struct ray;

struct sphere {
	struct vector position;
	double radius;
};

struct object create_sphere(struct sphere *);
optional_vector sphere_ray_intersection(void *sphere, struct ray ray);
size_t sphere_ray_intersection_count(struct sphere *sphere, struct ray ray);
optional_vector sphere_hit_normal(void *sphere, struct vector hit_position);

bool sphere_ray_entering(void *sphere, struct ray ray);
bool sphere_ray_leaving(void *sphere, struct ray ray);

#endif