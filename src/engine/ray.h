#ifndef RAY_H
#define RAY_H

#include <stddef.h>

#include "engine/object.h"
#include "engine/vector.h"

struct object;

struct ray {
    struct vector position;
    struct vector direction;
};

// todo: consider a more rich struct hit_data
// will store position, object, ray
optional_vector ray_get_nearest_hit_position(struct ray* ray,
                                             struct object objects[],
                                             size_t num_objects);

#endif
