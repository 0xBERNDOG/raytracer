#include "engine/ray.h"

#include <float.h>

#include "engine/object.h"

optional_hit_data
ray_get_hit_data(struct ray *ray, struct object objects[], size_t num_objects)
{
	optional_hit_data result = { .present = false };
	double min_dist = DBL_MAX;

	struct hit_data hit_data;

	for (size_t i = 0; i < num_objects; i++) {
		struct object object = objects[i];
		optional_vector hit_position =
			object.func_ray_intersection(object.object, ray);

		if (!hit_position.present) {
			continue;
		}

		// (p - r) . n
		// distance that hit position (p) is along ray direction (n)
		double dist_to_hit_pos = vector_dot(
			vector_subtract(hit_position.value, ray->position),
			vector_normalise(ray->direction));

		if (!result.present || dist_to_hit_pos < min_dist) {
			hit_data.object = &object;
			hit_data.position = hit_position.value;
			hit_data.brightness = object.brightness;
			// hit_data.normal = // todo

			min_dist = dist_to_hit_pos;

			result.present = true;
			result.value = hit_data;
			continue;
		}
	}

	return result;
}
