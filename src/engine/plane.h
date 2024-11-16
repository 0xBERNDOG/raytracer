#ifndef PLANE_H
#define PLANE_H

#include "engine/vector.h"

struct ray;

struct plane {
	struct vector position;
	struct vector normal;
};

struct object create_plane(struct plane *plane);
optional_vector plane_ray_intersection(void *plane, struct ray *ray);
optional_vector plane_hit_normal(void *plane, struct vector *hit_position);

#endif