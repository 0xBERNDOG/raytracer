#ifndef APERTURE_H
#define APERTURE_H

#include "engine/ray.h"
#include "engine/vector.h"

struct ray;

struct aperture {
	struct vector position;
	struct vector normal;

	double r_opening;
	double r_size;
};

struct object create_aperture(struct aperture *aperture);
optional_vector aperture_ray_intersection(void *aperture, struct ray ray);
optional_vector aperture_hit_normal(void *lens, struct vector hit_position);

bool aperture_ray_entering(void *aperture, struct ray ray);
bool aperture_ray_leaving(void *aperture, struct ray ray);

#endif