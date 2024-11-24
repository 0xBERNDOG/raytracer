#include "engine/lens.h"

#include "engine/ray.h"
#include "minunit.h"
#include "tests/engine/lens.h"

static char *
test_intersection()
{
	struct ray ray = { .position = { 0.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct lens lens = { .position = { 0.0, 0.0, 5.0 },
		             .normal = { 0.0, 0.0, 1.0 },
		             .r1 = 3.0,
		             .r2 = 3.0,
		             .separation = 1.0 };

	optional_vector result = lens_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 0.0, .y = 0.0, .z = 2.5 };
	mu_assert("lens-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_intersection_offset()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 1.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct lens lens = { .position = { 0.0, 0.0, 5.0 },
		             .normal = { 0.0, 1.0, 1.0 },
		             .r1 = 3.0,
		             .r2 = 3.0,
		             .separation = 1.0 };

	optional_vector result = lens_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	return 0;
}

static char *
test_intersection_from_inside_sphere1()
{
	struct ray ray = { .position = { .x = 0, .y = 0.0, .z = 1.5 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct lens lens = { .position = { 0.0, 0.0, 5.0 },
		             .normal = { 0.0, 0.0, 1.0 },
		             .r1 = 3.0,
		             .r2 = 3.0,
		             .separation = 1.0 };

	optional_vector result = lens_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 0.0, .y = 0.0, .z = 2.5 };
	mu_assert("lens-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_intersection_from_inside_lens()
{
	struct ray ray = { .position = { .x = 0, .y = 0.0, .z = 5.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct lens lens = { .position = { 0.0, 0.0, 5.0 },
		             .normal = { 0.0, 0.0, 1.0 },
		             .r1 = 3.0,
		             .r2 = 3.0,
		             .separation = 1.0 };

	optional_vector result = lens_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 0.0, .y = 0.0, .z = 7.5 };
	mu_assert("lens-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_no_intersection()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct lens lens = { .position = { 0.0, 0.0, 5.0 },
		             .normal = { 0.0, 0.0, 1.0 },
		             .r1 = 3.0,
		             .r2 = 3.0,
		             .separation = 1.0 };

	optional_vector result = lens_ray_intersection(&lens, ray);
	mu_assert("lens-ray intersection", result.present == false);

	return 0;
}

char *
test_lens_all()
{
	// run all lens tests
	mu_run_test(test_intersection);
	mu_run_test(test_intersection_offset);
	mu_run_test(test_intersection_from_inside_sphere1);
	mu_run_test(test_intersection_from_inside_lens);
	mu_run_test(test_no_intersection);
	// mu_run_test(test_hit_normal);
	// mu_run_test(test_hit_normal_no_hit);
	// mu_run_test(test_ray_entering);
	// mu_run_test(test_ray_leaving);
	return 0;
}
