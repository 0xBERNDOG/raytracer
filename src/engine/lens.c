#include "engine/lens.h"
#include "engine/ray.h"
#include "engine/sphere.h"
#include "utils/compare.h"

#include <assert.h>
#include <float.h>
#include <math.h>

struct object
create_lens(struct lens *lens)
{
	assert(lens != NULL);
	struct object object = { .object = (void *)lens,
		                 .func_ray_intersection =
		                         &lens_ray_intersection,
		                 .func_hit_normal = &lens_hit_normal,
		                 .func_ray_entering = &lens_ray_entering,
		                 .func_ray_leaving = &lens_ray_leaving };

	// make sure lens' normal is normalised
	lens->normal = vector_normalise(lens->normal);

	return object;
}

optional_vector
lens_ray_intersection(void *_lens, struct ray ray)
{
	// lens intersection is just two spheres
	// https://mathworld.wolfram.com/Sphere-SphereIntersection.html

	optional_vector result = { .present = false };
	struct lens *lens = (struct lens *)_lens;

	// todo: support for more complex lenses with negative radius
	assert(lens->r1 > 0.0);
	assert(lens->r2 > 0.0);

	double x = (pow(lens->separation, 2) - pow(lens->r2, 2) +
	            pow(lens->r1, 2)) /
	           (2 * lens->separation);
	struct vector pos1 =
		vector_add(lens->position, vector_multiply(lens->normal, x));
	struct vector pos2 =
		vector_add(lens->position,
	                   vector_multiply(lens->normal, x - lens->separation));

	struct sphere sphere1 = { .position = pos1, .radius = lens->r1 };
	struct sphere sphere2 = { .position = pos2, .radius = lens->r2 };

	// get the closest intersection points for each sphere
	// then find the one that is also within/on the other sphere
	// if there are any results, take the closest one

	optional_vector hit1 = sphere_ray_intersection(&sphere1, ray);
	optional_vector hit2 = sphere_ray_intersection(&sphere2, ray);

	if (hit1.present == false && hit2.present == false) {
		return result;
	}

	double min_dist = DBL_MAX;
	if (hit1.present) {
		double dist_to_hit_pos = vector_len(
			vector_subtract(hit1.value, sphere2.position));
		if (FUZZY_LT(dist_to_hit_pos, sphere2.radius)) {
			// hit1 is on the lens
			if (result.present == false ||
			    dist_to_hit_pos < min_dist) {
				min_dist = dist_to_hit_pos;
				result.present = true;
				result.value = hit1.value;
			}
		}
	}

	if (hit2.present) {
		double dist_to_hit_pos = vector_len(
			vector_subtract(hit2.value, sphere1.position));
		if (FUZZY_LT(dist_to_hit_pos, sphere1.radius)) {
			// hit2 is on the lens
			if (result.present == false ||
			    dist_to_hit_pos < min_dist) {
				min_dist = dist_to_hit_pos;
				result.present = true;
				result.value = hit2.value;
			}
		}
	}

	return result;
}

optional_vector
lens_hit_normal(void *lens, struct vector hit_position)
{
	optional_vector result = { .present = false };
	return result;
}

bool
lens_ray_entering(void *lens, struct ray ray)
{
	return false;
}

bool
lens_ray_leaving(void *lens, struct ray ray)
{
	return false;
}