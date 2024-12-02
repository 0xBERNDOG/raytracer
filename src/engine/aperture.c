#include "engine/aperture.h"

#include <assert.h>
#include <math.h>

#include "engine/object.h"
#include "engine/plane.h"
#include "engine/ray.h"
#include "utils/compare.h"

static struct plane
aperture_to_plane(struct aperture *aperture)
{
	return (struct plane) { .position = aperture->position,
		                .normal = aperture->normal };
}

static bool
point_on_surface(struct aperture *aperture, struct vector point)
{
	struct plane plane = aperture_to_plane(aperture);

	struct vector relative_hit_position =
		vector_subtract(point, plane.position);
	double vertical_dist =
		fabs(vector_dot(relative_hit_position, plane.normal));

	if (!FUZZY_EQUALS(vertical_dist, 0.0)) {
		// too far away from plane's surface to be a hit
		return false;
	}

	double radial_dist = vector_len(relative_hit_position);
	if ((aperture->r_size != 0 &&
	     !FUZZY_LT(radial_dist, aperture->r_size)) ||
	    FUZZY_LT(radial_dist, aperture->r_opening)) {
		// inside the opening
		return false;
	}

	return true;
}

optional_vector
aperture_ray_intersection(void *_aperture, struct ray ray)
{
	struct aperture *aperture = (struct aperture *)_aperture;
	struct plane plane = aperture_to_plane(aperture);

	optional_vector result = plane_ray_intersection(&plane, ray);
	if (result.present && point_on_surface(aperture, result.value)) {
		return result;
	}

	return (optional_vector) { .present = false };
}

optional_vector
aperture_hit_normal(void *_aperture, struct vector hit_position)
{
	struct aperture *aperture = (struct aperture *)_aperture;
	optional_vector result = { .present = false };

	if (point_on_surface(aperture, hit_position)) {
		result.present = true;
		result.value = aperture->normal;
	}

	return result;
}

bool
aperture_ray_entering(void *aperture, struct ray ray)
{
	// apertures are infinitely thin, so it doesn't make sense to be
	// entering (or leaving) it
	(void)aperture;    // unused
	(void)ray;         // unused
	return false;
}

bool
aperture_ray_leaving(void *aperture, struct ray ray)
{
	// apertures are infinitely thin, so it doesn't make sense to be
	// entering (or leaving) it
	(void)aperture;    // unused
	(void)ray;         // unused
	return false;
}

struct object
create_aperture(struct aperture *aperture)
{
	assert(aperture != NULL);
	struct object object = { .object = (void *)aperture,
		                 .func_ray_intersection =
		                         &aperture_ray_intersection,
		                 .func_ray_refraction =
		                         &object_default_refraction,
		                 .func_hit_normal = &aperture_hit_normal,
		                 .func_ray_entering = &aperture_ray_entering,
		                 .func_ray_leaving = &aperture_ray_leaving };

	// make sure plane's normal is normalised
	aperture->normal = vector_normalise(aperture->normal);

	return object;
}