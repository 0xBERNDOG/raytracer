#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "vector.h"

struct object {
    optional_vector (*func_ray_intersection)(
        struct ray *);  // function that takes a ray and returns point of
                        // intersection (if there is one)
};

#endif
