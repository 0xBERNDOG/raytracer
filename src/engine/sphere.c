#include "engine/ray.h"
#include "engine/sphere.h"

#include <math.h>

optional_vector sphere_ray_intersection(void* _sphere, struct ray* ray) {
    // https://en.wikipedia.org/wiki/Line-sphere_intersection
    optional_vector result = {.present = false};
    struct sphere* sphere = (struct sphere*)_sphere;

    struct vector ray_dir = vector_normalise(ray->direction);

    struct vector diff = vector_subtract(ray->position, sphere->position);
    double dot = vector_dot(ray_dir, diff);
    double discriminant =
        pow(dot, 2) - (pow(vector_len(diff), 2) - pow(sphere->radius, 2));

    double tolerance = 0.0001;
    if (discriminant < -tolerance) {
        // no intersection
        return result;
    }

    if (fabs(discriminant) <= tolerance) {
        discriminant = 0.0;
    }

    double d1 = -dot + sqrt(discriminant);
    double d2 = -dot - sqrt(discriminant);
    double d;

    if (discriminant == 0.0) {
        // single intersection, both are equal
        d = d1;
    } else {
        // two solutions, take the smaller one in the +ve direction
        if (d1 >= 0.0 && d2 >= 0.0) {
            // take the smaller one
            if (d1 < d2) {
                d = d1;
            } else {
                d = d2;
            }
        } else {
            // take the non-negative one
            if (d1 < 0) {
                d = d2;
            } else {
                d = d1;
            }
        }
    }

    struct vector ray_offset = vector_multiply(ray_dir, d);
    struct vector intersection = vector_add(ray->position, ray_offset);

    result.value = intersection;
    result.present = true;

    return result;
}