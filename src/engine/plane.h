#ifndef PLANE_H
#define PLANE_H

#include "engine/ray.h"
#include "engine/vector.h"

struct plane {
    struct vector position;
    struct vector normal;
};

optional_vector plane_ray_intersection(void* plane, struct ray* ray);

#endif