#include "engine/lens_ideal.h"

#include "engine/ray.h"
#include "minunit.h"
#include "tests/engine/lens_ideal.h"

static char *
test_intersection()
{
	struct ray ray = { .position = { 0.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct lens_ideal lens = { .position = { 0.0, 0.0, 5.0 },
		                   .normal = { 0.0, 0.0, 1.0 },
		                   .r = 3.0,
		                   .f = 3.0 };

	optional_vector result = lens_ideal_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 0.0, .y = 0.0, .z = 5.0 };
	mu_assert("lens-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_intersection_angled()
{
	struct ray ray = { .position = { 0.0, 0.0, 0.0 },
		           .direction = { 0.0, 1.0, 1.0 } };

	struct lens_ideal lens = { .position = { 0.0, 0.0, 1.0 },
		                   .normal = { 1.0, 1.0, 1.0 },
		                   .r = 3.0,
		                   .f = 3.0 };

	optional_vector result = lens_ideal_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	return 0;
}

static char *
test_intersection_offset()
{
	struct ray ray = { .position = { 1.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct lens_ideal lens = { .position = { 0.0, 0.0, 5.0 },
		                   .normal = { 0.0, 0.0, 1.0 },
		                   .r = 3.0,
		                   .f = 3.0 };

	optional_vector result = lens_ideal_ray_intersection(&lens, ray);
	mu_assert("no lens-ray intersection", result.present == true);

	ray.position = (struct vector) { 5.0, 0.0, 0.0 };
	result = lens_ideal_ray_intersection(&lens, ray);
	mu_assert("lens-ray intersection", result.present == false);

	return 0;
}

static char *
test_no_intersection()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct lens_ideal lens = { .position = { .x = 1.0, .y = 0.0, .z = 0.0 },
		                   .normal = { .x = 1.0, .y = 0.0, .z = 0.0 },
		                   .r = 3.0,
		                   .f = 3.0 };

	optional_vector result = lens_ideal_ray_intersection(&lens, ray);
	mu_assert("lens-ray intersection", result.present == false);

	return 0;
}

static char *
test_hit_normal()
{
	struct lens_ideal lens = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		                   .normal = { 0.0, 0.0, 1.0 },
		                   .r = 3.0,
		                   .f = 3.0 };
	struct vector hit_position = { 1.0, 0.0, 0.0 };
	struct vector expected = { 0.0, 0.0, 1.0 };

	optional_vector result = lens_ideal_hit_normal(&lens, hit_position);
	mu_assert("no lens hit normal", result.present == true);
	mu_assert("lens hit normal not equal to expected",
	          vector_equals(result.value, expected));

	return 0;
}

static char *
test_hit_normal_no_hit()
{
	struct lens_ideal lens = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		                   .normal = { 0.0, 0.0, 1.0 },
		                   .r = 3.0,
		                   .f = 3.0 };
	struct vector hit_position = { 1.0, 0.0, 1.0 };

	optional_vector result = lens_ideal_hit_normal(&lens, hit_position);
	mu_assert("unexpected lens hit normal", result.present == false);

	result = lens_ideal_hit_normal(&lens, hit_position);
	mu_assert("unexpected lens hit normal", result.present == false);

	return 0;
}

static char *
test_ray_entering()
{
	struct lens_ideal lens = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		                   .normal = { 1.0, 0.0, 0.0 },
		                   .r = 3.0,
		                   .f = 3.0 };
	struct ray ray = { .position = { 1.0, 0.0, 0.0 },
		           .direction = { 1.0, 0.0, 0.0 } };

	bool result = lens_ideal_ray_entering(&lens, ray);
	mu_assert("ray entering lens", result == false);

	return 0;
}

static char *
test_ray_leaving()
{
	struct lens_ideal lens = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		                   .normal = { 1.0, 0.0, 0.0 },
		                   .r = 3.0,
		                   .f = 3.0 };
	struct ray ray = { .position = { 1.0, 0.0, 0.0 },
		           .direction = { 1.0, 0.0, 0.0 } };

	bool result = lens_ideal_ray_leaving(&lens, ray);
	mu_assert("ray leaving lens", result == false);

	return 0;
}

char *
test_lens_ideal_all()
{
	// run all lens tests
	mu_run_test(test_intersection);
	mu_run_test(test_intersection_angled);
	mu_run_test(test_intersection_offset);
	mu_run_test(test_no_intersection);
	mu_run_test(test_hit_normal);
	mu_run_test(test_hit_normal_no_hit);
	mu_run_test(test_ray_entering);
	mu_run_test(test_ray_leaving);

	return 0;
}
