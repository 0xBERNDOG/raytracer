#include "engine/plane.h"

#include <assert.h>
#include <math.h>

#include "engine/ray.h"
#include "utils/compare.h"

optional_vector
plane_ray_intersection(void *_plane, struct ray ray)
{
	// https://en.wikipedia.org/wiki/Line-plane_intersection
	optional_vector result = { .present = false };
	struct plane *plane = (struct plane *)_plane;

	if (vector_equals(plane->position, ray.position)) {
		result.value = ray.position;
		result.present = true;

		return result;
	}

	struct vector plane_norm = vector_normalise(plane->normal);
	struct vector ray_dir = vector_normalise(ray.direction);

	struct vector difference =
		vector_subtract(plane->position, ray.position);
	double numerator = vector_dot(difference, plane_norm);
	double denominator = vector_dot(ray_dir, plane_norm);

	if (denominator == 0) {
		// plane and ray are parallel

		if (numerator == 0) {
			// ray is inside the plane
			result.value = ray.position;
			result.present = true;

			return result;
		}

		// ray is outside the plane, so will never intersect
		return result;
	}

	double d = numerator / denominator;

	if (d < 0.0) {
		// no solution in front of ray
		return result;
	}

	struct vector ray_offset = vector_multiply(ray_dir, d);
	struct vector intersection = vector_add(ray.position, ray_offset);

	result.value = intersection;
	result.present = true;

	return result;
}

optional_vector
plane_hit_normal(void *_plane, struct vector hit_position)
{
	optional_vector result = { .present = false };
	struct plane *plane = (struct plane *)_plane;

	struct vector relative_hit_position =
		vector_subtract(hit_position, plane->position);
	double vertical_dist =
		fabs(vector_dot(relative_hit_position, plane->normal));

	if (!FUZZY_EQUALS(vertical_dist, 0.0)) {
		// too far away from plane's surface to be a hit
		return result;
	}

	result.value = plane->normal;
	result.present = true;

	return result;
}

bool
plane_ray_entering(void *plane, struct ray ray)
{
	// planes are infinitely thin, so it doesn't make sense to be entering
	// (or leaving) it
	(void)plane;    // unused
	(void)ray;      // unused
	return false;
}

bool
plane_ray_leaving(void *plane, struct ray ray)
{
	// planes are infinitely thin, so it doesn't make sense to be entering
	// (or leaving) it
	(void)plane;    // unused
	(void)ray;      // unused
	return false;
}

struct object
create_plane(struct plane *plane)
{
	assert(plane != NULL);
	struct object object = { .object = (void *)plane,
		                 .func_ray_intersection =
		                         &plane_ray_intersection,
		                 .func_hit_normal = &plane_hit_normal,
		                 .func_ray_entering = &plane_ray_entering,
		                 .func_ray_leaving = &plane_ray_leaving };

	// make sure plane's normal is normalised
	plane->normal = vector_normalise(plane->normal);

	return object;
}