#include "engine/plane.h"

optional_vector plane_ray_intersection(void* _plane, struct ray* ray) {
    // https://en.wikipedia.org/wiki/Line-plane_intersection

    optional_vector result = {.present = false};

    struct plane* plane = (struct plane*)_plane;
    struct vector difference = vector_subtract(plane->position, ray->position);
    float numerator = vector_dot(difference, plane->normal);
    float denominator = vector_dot(ray->direction, plane->normal);

    if (denominator == 0) {
        // plane and ray are parallel, so will never intersect
        return result;
    }

    float d = numerator / denominator;
    struct vector ray_offset = vector_multiply(ray->direction, d);
    struct vector intersection = vector_add(ray->position, ray_offset);

    result.value = intersection;
    result.present = true;

    return result;
}