#include "engine/lens.h"
#include "engine/ray.h"
#include "engine/sphere.h"
#include "utils/compare.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

static struct sphere *
lens_to_spheres(struct lens lens)
{
	struct sphere *spheres = malloc(2 * sizeof(struct sphere));
	assert(spheres);

	double x =
		(pow(lens.separation, 2) - pow(lens.r2, 2) + pow(lens.r1, 2)) /
		(2 * lens.separation);
	struct vector pos1 =
		vector_add(lens.position, vector_multiply(lens.normal, x));
	struct vector pos2 =
		vector_add(lens.position,
	                   vector_multiply(lens.normal, x - lens.separation));

	spheres[0].position = pos1;
	spheres[0].radius = lens.r1;

	spheres[1].position = pos2;
	spheres[1].radius = lens.r2;

	return spheres;
}

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

	struct sphere *spheres = lens_to_spheres(*lens);
	struct sphere sphere1 = spheres[0];
	struct sphere sphere2 = spheres[1];

	// get the closest intersection points for each sphere
	// then find the one that is also within/on the other sphere
	// if there are any results, take the closest one

	optional_vector hit1 = sphere_ray_intersection(&sphere1, ray);
	optional_vector hit2 = sphere_ray_intersection(&sphere2, ray);

	if (hit1.present == false && hit2.present == false) {
		free(spheres);
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

	free(spheres);
	return result;
}

optional_vector
lens_hit_normal(void *_lens, struct vector hit_position)
{
	optional_vector result = { .present = false };
	struct lens *lens = (struct lens *)_lens;

	// need to be on the surface of sphere1 && inside sphere2 (or vice
	// versa)

	struct sphere *spheres = lens_to_spheres(*lens);
	struct sphere sphere1 = spheres[0];
	struct sphere sphere2 = spheres[1];

	// surface of sphere1, inside of sphere2 (farside)
	double dist_to_hit_pos =
		vector_len(vector_subtract(hit_position, sphere2.position));
	if (FUZZY_LT(dist_to_hit_pos, sphere2.radius)) {
		result = sphere_hit_normal(&sphere1, hit_position);
	}

	if (!result.present) {
		// surface of sphere2, inside of sphere1 (nearside)
		dist_to_hit_pos = vector_len(
			vector_subtract(hit_position, sphere1.position));
		if (FUZZY_LT(dist_to_hit_pos, sphere1.radius)) {
			result = sphere_hit_normal(&sphere2, hit_position);
		}
	}

	free(spheres);
	return result;
}

bool
lens_ray_entering(void *_lens, struct ray ray)
{
	// if total sphere_ray intersections = 4, we are always entering;
	// if total sphere_ray intersections = 3, and we are inside the sphere
	// with 1, then we are entering;
	// else we aren't entering
	bool result = false;
	struct lens *lens = (struct lens *)_lens;
	struct sphere *spheres = lens_to_spheres(*lens);

	struct sphere sphere1 = spheres[0];
	struct sphere sphere2 = spheres[1];

	size_t sphere1_hits = sphere_ray_intersection_count(&sphere1, ray);
	size_t sphere2_hits = sphere_ray_intersection_count(&sphere2, ray);

	if (sphere1_hits + sphere2_hits == 4) {
		result = true;
	} else {
		if (sphere1_hits == 1 && sphere2_hits == 2) {
			double sphere1_to_ray = vector_len(vector_subtract(
				ray.position, sphere1.position));
			bool inside_sphere1 =
				FUZZY_LT(sphere1_to_ray, sphere1.radius);
			result = inside_sphere1;
		} else if (sphere1_hits == 2 && sphere2_hits == 1) {
			double sphere2_to_ray = vector_len(vector_subtract(
				ray.position, sphere2.position));
			bool inside_sphere2 =
				FUZZY_LT(sphere2_to_ray, sphere2.radius);
			result = inside_sphere2;
		}
	}

	free(spheres);
	return result;
}

bool
lens_ray_leaving(void *_lens, struct ray ray)
{
	// simply requires one sphere_ray intersection each
	bool result = false;
	struct lens *lens = (struct lens *)_lens;
	struct sphere *spheres = lens_to_spheres(*lens);

	struct sphere sphere1 = spheres[0];
	struct sphere sphere2 = spheres[1];

	size_t sphere1_hits = sphere_ray_intersection_count(&sphere1, ray);
	size_t sphere2_hits = sphere_ray_intersection_count(&sphere2, ray);

	result = (sphere1_hits == 1 && sphere2_hits == 1);

	free(spheres);
	return result;
}