#include "engine/sphere.h"

#include <math.h>

#include "engine/ray.h"
#include "minunit.h"
#include "tests/engine/sphere.h"
#include "utils/compare.h"

static char *
test_intersection()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct sphere sphere = { .position = { .x = 5.0, .y = 0.0, .z = 0.0 },
		                 .radius = 1.0 };

	optional_vector result = sphere_ray_intersection(&sphere, &ray);
	mu_assert("no sphere-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 4.0, .y = 0.0, .z = 0.0 };
	mu_assert("sphere-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_offset()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.5 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct sphere sphere = { .position = { .x = 5.0, .y = 0.0, .z = 0.0 },
		                 .radius = 1.0 };

	optional_vector result = sphere_ray_intersection(&sphere, &ray);
	mu_assert("no sphere-ray intersection", result.present == true);

	struct vector intersection = result.value;
	struct vector diff = vector_subtract(sphere.position, intersection);
	float expected = 1.0;

	mu_assert("sphere-ray intersection distance not equal to expected",
	          FUZZY_EQUALS(vector_len(diff), expected));

	return 0;
}

static char *
test_intersection_glancing()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 1.0 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct sphere sphere = { .position = { .x = 5.0, .y = 0.0, .z = 0.0 },
		                 .radius = 1.0 };

	optional_vector result = sphere_ray_intersection(&sphere, &ray);
	mu_assert("no sphere-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 5.0, .y = 0.0, .z = 1.0 };
	mu_assert("sphere-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_intersection_from_inside()
{
	struct ray ray = { .position = { .x = 4.5, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct sphere sphere = { .position = { .x = 5.0, .y = 0.0, .z = 0.0 },
		                 .radius = 1.0 };

	optional_vector result = sphere_ray_intersection(&sphere, &ray);
	mu_assert("no sphere-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 6.0, .y = 0.0, .z = 0.0 };
	mu_assert("sphere-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_no_intersection()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 5.0 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct sphere sphere = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		                 .radius = 1.0 };

	optional_vector result = sphere_ray_intersection(&sphere, &ray);
	mu_assert("sphere-ray intersection", result.present == false);

	return 0;
}

char *
test_sphere_all()
{
	// run all sphere tests
	mu_run_test(test_intersection);
	mu_run_test(test_offset);
	mu_run_test(test_intersection_glancing);
	mu_run_test(test_intersection_from_inside);
	mu_run_test(test_no_intersection);
	return 0;
}
