#ifndef SPHERE_H
#define SPHERE_H

#include "engine/vector.h"

struct ray;

struct sphere {
	struct vector position;
	double radius;
};

optional_vector sphere_ray_intersection(void *sphere, struct ray *ray);

#endif