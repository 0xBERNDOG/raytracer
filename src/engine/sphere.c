#include "engine/sphere.h"

#include <assert.h>
#include <math.h>

#include "engine/ray.h"
#include "utils/compare.h"

optional_vector
sphere_ray_intersection(void *_sphere, struct ray *ray)
{
	// https://en.wikipedia.org/wiki/Line-sphere_intersection
	optional_vector result = { .present = false };
	struct sphere *sphere = (struct sphere *)_sphere;

	struct vector ray_dir = vector_normalise(ray->direction);

	struct vector diff = vector_subtract(ray->position, sphere->position);
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

struct object
create_sphere(struct sphere *sphere)
{
	assert(sphere != NULL);
	struct object object = { .object = (void *)sphere,
		                 .func_ray_intersection =
		                         &sphere_ray_intersection };
	return object;
}