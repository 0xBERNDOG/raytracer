#include "engine/plane.h"

optional_vector plane_ray_intersection(void* _plane, struct ray* ray) {
    // https://en.wikipedia.org/wiki/Line-plane_intersection

    optional_vector result = {.present = false};
    struct plane* plane = (struct plane*)_plane;

    if (vector_equals(plane->position, ray->position)) {
        result.value = ray->position;
        result.present = true;

        return result;
    }

    vector plane_norm = vector_normalise(plane->normal);
    vector ray_dir = vector_normalise(ray->direction);

    vector difference = vector_subtract(plane->position, ray->position);
    float numerator = vector_dot(difference, plane_norm);
    float denominator = vector_dot(ray_dir, plane_norm);

    if (denominator == 0) {
        // plane and ray are parallel

        if (numerator == 0) {
            // ray is inside the plane
            result.value = ray->position;
            result.present = true;

            return result;
        }

        // ray is outside the plane, so will never intersect
        return result;
    }

    float d = numerator / denominator;
    vector ray_offset = vector_multiply(ray_dir, d);
    vector intersection = vector_add(ray->position, ray_offset);

    result.value = intersection;
    result.present = true;

    return result;
}