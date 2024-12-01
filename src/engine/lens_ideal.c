#include "engine/lens_ideal.h"
#include "engine/object.h"
#include "engine/plane.h"
#include "engine/ray.h"
#include "utils/compare.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

struct object
create_lens_ideal(struct lens_ideal *lens)
{
	assert(lens != NULL);
	struct object object = { .object = (void *)lens,
		                 .func_ray_intersection =
		                         &lens_ideal_ray_intersection,
		                 .func_ray_refraction = &lens_ideal_refraction,
		                 .func_hit_normal = &lens_ideal_hit_normal,
		                 .func_ray_entering = &lens_ideal_ray_entering,
		                 .func_ray_leaving = &lens_ideal_ray_leaving };

	// make sure lens' normal is normalised
	lens->normal = vector_normalise(lens->normal);

	return object;
}

optional_vector
lens_ideal_ray_intersection(void *_lens, struct ray ray)
{
	struct lens_ideal *lens = (struct lens_ideal *)_lens;

	// todo: support for negative radius (divergent lens)
	assert(lens->r >= 0.0);    // r = 0 is an infinitely big lens

	struct plane _plane = { .position = lens->position,
		                .normal = lens->normal };
	struct object plane = create_plane(&_plane);

	optional_vector result = plane.func_ray_intersection(plane.object, ray);

	if (result.present && lens->r != 0.0) {
		// need to check if hit position is within the lens' radius
		struct vector diff =
			vector_subtract(lens->position, result.value);
		double dist = vector_len(diff);

		if (dist > lens->r) {
			return (optional_vector) { .present = false };
		}
	}

	return result;
}

optional_vector
lens_ideal_hit_normal(void *_lens, struct vector hit_position)
{
	struct lens_ideal *lens = (struct lens_ideal *)_lens;

	// todo: support for negative radius (divergent lens)
	assert(lens->r >= 0.0);    // r = 0 is an infinitely big lens

	struct plane _plane = { .position = lens->position,
		                .normal = lens->normal };
	struct object plane = create_plane(&_plane);

	optional_vector result =
		plane.func_hit_normal(plane.object, hit_position);

	if (result.present && lens->r != 0.0) {
		// need to check if hit position is within the lens' radius
		struct vector diff =
			vector_subtract(lens->position, result.value);
		double dist = vector_len(diff);

		if (dist > lens->r) {
			return (optional_vector) { .present = false };
		}
	}

	return result;
}

bool
lens_ideal_ray_entering(void *lens, struct ray ray)
{
	// ideal lenses are infinitely thin, so it doesn't make sense to be
	// entering (or leaving) it
	(void)lens;    // unused
	(void)ray;     // unused
	return false;
}

bool
lens_ideal_ray_leaving(void *lens, struct ray ray)
{
	// ideal lenses are infinitely thin, so it doesn't make sense to be
	// entering (or leaving) it
	(void)lens;    // unused
	(void)ray;     // unused
	return false;
}

optional_ray
lens_ideal_refraction(struct ray ray, struct hit_data data)
{
	struct object *lens_object = data.object;
	struct lens_ideal *lens = (struct lens_ideal *)lens_object->object;

	// todo: support for negative radius (divergent lens)
	assert(lens->r >= 0.0);    // r = 0 is an infinitely big lens
	assert(lens->f > 0.0);

	struct vector d = vector_subtract(data.position, lens->position);
	double d_len = vector_len(d);

	assert(d_len <= lens->r);

	struct ray new_ray = (struct ray) { .position = data.position,
		                            .direction = ray.direction };
	if (FUZZY_EQUALS(d_len, 0.0)) {
		return (optional_ray) { .present = true, .value = new_ray };
	}

	struct vector deflection = vector_multiply(d, -1.0 / lens->f);
	struct vector new_direction =
		vector_normalise(vector_add(ray.direction, deflection));

	new_ray.direction = new_direction;
	return (optional_ray) { .present = true, .value = new_ray };
}