#include "engine/plane.h"

#include <assert.h>

#include "engine/ray.h"

optional_vector
plane_ray_intersection(void *_plane, struct ray *ray)
{
	// https://en.wikipedia.org/wiki/Line-plane_intersection
	optional_vector result = { .present = false };
	struct plane *plane = (struct plane *)_plane;

	if (vector_equals(plane->position, ray->position)) {
		result.value = ray->position;
		result.present = true;

		return result;
	}

	struct vector plane_norm = vector_normalise(plane->normal);
	struct vector ray_dir = vector_normalise(ray->direction);

	struct vector difference =
		vector_subtract(plane->position, ray->position);
	double numerator = vector_dot(difference, plane_norm);
	double denominator = vector_dot(ray_dir, plane_norm);

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

	double d = numerator / denominator;
	struct vector ray_offset = vector_multiply(ray_dir, d);
	struct vector intersection = vector_add(ray->position, ray_offset);

	result.value = intersection;
	result.present = true;

	return result;
}

struct object
create_plane(struct plane *plane)
{
	assert(plane != NULL);
	struct object object = { .object = (void *)plane,
		                 .func_ray_intersection =
		                         &plane_ray_intersection };

	return object;
}