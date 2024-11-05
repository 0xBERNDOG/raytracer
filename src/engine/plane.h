#ifndef PLANE_H
#define PLANE_H

#include "engine/vector.h"

struct ray;

struct plane {
    struct vector position;
    struct vector normal;
};

optional_vector plane_ray_intersection(void* plane, struct ray* ray);

#endif