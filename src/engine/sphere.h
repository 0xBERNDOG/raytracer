#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "vector.h"

struct sphere {
    struct vector position;
    float radius;
};

optional_vector sphere_ray_intersection(void* sphere, struct ray* ray);

#endif