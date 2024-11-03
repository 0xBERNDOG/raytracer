#ifndef PLANE_H
#define PLANE_H

#include "ray.h"
#include "vector.h"

struct plane {
    vector position;
    vector normal;
};

optional_vector plane_ray_intersection(void* plane, struct ray* ray);

#endif