#include "engine/ray.h"

#include <assert.h>
#include <float.h>

#include "engine/object.h"
#include "utils/compare.h"

optional_hit_data
ray_get_hit_data(struct ray ray, struct object *objects, size_t num_objects,
                 struct object *last_hit)
{
	optional_hit_data result = { .present = false };
	double min_dist = DBL_MAX;

	struct hit_data hit_data;

	for (size_t i = 0; i < num_objects; i++) {
		struct object *object = &objects[i];

		optional_vector hit_position =
			object->func_ray_intersection(object->object, ray);

		if (!hit_position.present) {
			continue;
		}

		// (p - r) . n
		// distance that hit position (p) is along ray direction (n)
		double dist_to_hit_pos = vector_dot(
			vector_subtract(hit_position.value, ray.position),
			vector_normalise(ray.direction));

		// extra distance required for last_hit object to avoid
		// self-intersection
		if (object == last_hit &&
		    dist_to_hit_pos < FUZZY_EQUALS_TOLERANCE) {
			continue;
		}

		if (!result.present || dist_to_hit_pos < min_dist) {
			hit_data.object = object;
			hit_data.position = hit_position.value;
			hit_data.brightness = object->brightness;
			hit_data.reflectivity = object->reflectivity;
			hit_data.transmissivity = object->transmissivity;
			hit_data.refractive_index = object->refractive_index;

			min_dist = dist_to_hit_pos;
			result.present = true;
			continue;
		}
	}

	if (result.present) {
		struct object *object = hit_data.object;
		optional_vector hit_normal = object->func_hit_normal(
			object->object, hit_data.position);
		assert(hit_normal.present);
		hit_data.normal = hit_normal.value;

		result.value = hit_data;
	}

	return result;
}
