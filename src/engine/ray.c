#include "engine/ray.h"

#include "engine/object.h"

optional_vector ray_get_nearest_hit_position(struct ray* ray,
                                             struct object objects[],
                                             size_t num_objects) {
    optional_vector result = {.present = false};
    double min_dist;

    for (size_t i = 0; i < num_objects; i++) {
        struct object object = objects[i];
        optional_vector hit_result =
            object.func_ray_intersection(object.object, ray);

        if (!hit_result.present) {
            continue;
        }

        // (p - r) . n
        // distance that hit position (p) is along ray direction (n)
        double dist_to_hit_pos =
            vector_dot(vector_subtract(hit_result.value, ray->position),
                       vector_normalise(ray->direction));

        if (!result.present) {
            result = hit_result;
            min_dist = dist_to_hit_pos;
            continue;
        }

        if (dist_to_hit_pos < min_dist) {
            result = hit_result;
            min_dist = dist_to_hit_pos;
        }
    }

    return result;
}