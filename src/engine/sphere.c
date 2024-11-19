#include "engine/sphere.h"

#include <assert.h>
#include <math.h>

#include "engine/ray.h"
#include "utils/compare.h"

optional_vector
sphere_ray_intersection(void *_sphere, struct ray ray)
{
	// https://en.wikipedia.org/wiki/Line-sphere_intersection
	optional_vector result = { .present = false };
	struct sphere *sphere = (struct sphere *)_sphere;

	struct vector ray_dir = vector_normalise(ray.direction);

	struct vector diff = vector_subtract(ray.position, sphere->position);
	double dot = vector_dot(ray_dir, diff);
	double discriminant = pow(dot, 2) - (pow(vector_len(diff), 2) -
	                                     pow(sphere->radius, 2));

	if (discriminant < -FUZZY_EQUALS_TOLERANCE) {
		// no intersection
		return result;
	}

	if (FUZZY_EQUALS(discriminant, 0.0)) {
		discriminant = 0.0;
	}

	double d1 = -dot + sqrt(discriminant);
	double d2 = -dot - sqrt(discriminant);
	double d;

	if (discriminant == 0.0) {
		// single intersection, both are equal
		d = d1;
	} else {
		if (d1 < 0.0 && d2 < 0.0) {
			// no solution in front of ray
			return result;
		}

		// two solutions, take the smaller one in the +ve direction
		// (greater than a min amount to avoid spurious intersections)
		if (d1 >= FUZZY_EQUALS_TOLERANCE &&
		    d2 >= FUZZY_EQUALS_TOLERANCE) {
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
	struct vector intersection = vector_add(ray.position, ray_offset);

	result.value = intersection;
	result.present = true;

	return result;
}

static size_t
sphere_ray_intersection_count(struct sphere *sphere, struct ray ray)
{
	// return the number of intersections in front of the ray
	struct vector ray_dir = vector_normalise(ray.direction);

	struct vector diff = vector_subtract(ray.position, sphere->position);
	double dot = vector_dot(ray_dir, diff);
	double discriminant = pow(dot, 2) - (pow(vector_len(diff), 2) -
	                                     pow(sphere->radius, 2));

	if (discriminant < -FUZZY_EQUALS_TOLERANCE) {
		// no intersection
		return 0;
	}

	if (FUZZY_EQUALS(discriminant, 0.0)) {
		discriminant = 0.0;
	}

	double d1 = -dot + sqrt(discriminant);
	double d2 = -dot - sqrt(discriminant);

	if (discriminant == 0.0) {
		// single intersection, both are equal
		return 1.0;
	} else {
		if (d1 <= 0.0 && d2 <= 0.0) {
			// no solution in front of ray
			return 0;
		}

		if (d1 * d2 > 0.0) {
			// both d1, d2 +ve
			return 2;
		} else {
			// one of d1, d2 is -ve or zero
			return 1;
		}
	}

	return 0;
}

optional_vector
sphere_hit_normal(void *_sphere, struct vector hit_position)
{
	optional_vector result = { .present = false };
	struct sphere *sphere = (struct sphere *)_sphere;

	struct vector relative_hit_position =
		vector_subtract(hit_position, sphere->position);

	if (!FUZZY_EQUALS(vector_len(relative_hit_position), sphere->radius)) {
		// too far away from centre to be a hit
		return result;
	}

	result.value = vector_normalise(relative_hit_position);
	result.present = true;

	return result;
}

bool
sphere_ray_entering(void *_sphere, struct ray ray)
{
	struct sphere *sphere = (struct sphere *)_sphere;

	size_t num_intersections = sphere_ray_intersection_count(sphere, ray);
	return num_intersections > 1;
}

bool
sphere_ray_leaving(void *_sphere, struct ray ray)
{
	struct sphere *sphere = (struct sphere *)_sphere;

	size_t num_intersections = sphere_ray_intersection_count(sphere, ray);
	return num_intersections == 1;
}

struct object
create_sphere(struct sphere *sphere)
{
	assert(sphere != NULL);
	struct object object = { .object = (void *)sphere,
		                 .func_ray_intersection =
		                         &sphere_ray_intersection,
		                 .func_hit_normal = &sphere_hit_normal,
		                 .func_ray_entering = &sphere_ray_entering,
		                 .func_ray_leaving = &sphere_ray_leaving };
	return object;
}