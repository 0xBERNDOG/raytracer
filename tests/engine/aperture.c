#include "engine/aperture.h"

#include "engine/ray.h"
#include "minunit.h"
#include "tests/engine/aperture.h"

static char *
test_intersection()
{
	struct ray ray = { .position = { 4.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };

	optional_vector result = aperture_ray_intersection(&aperture, ray);
	mu_assert("no aperture-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { 4.0, 0.0, 1.0 };
	mu_assert("aperture-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	// r_size = 0 is an infinitely large aperture
	aperture.r_size = 0;
	ray.position = (struct vector) { 10.0, 0.0, 0.0 };
	result = aperture_ray_intersection(&aperture, ray);
	mu_assert("no aperture-ray intersection", result.present == true);

	intersection = result.value;
	expected = (struct vector) { 10.0, 0.0, 1.0 };
	mu_assert("aperture-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_intersection_angled()
{
	struct ray ray = { .position = { 0.0, 0.0, 0.0 },
		           .direction = { 0.0, 1.0, 1.0 } };

	struct aperture aperture = { .position = { 0.0, 0.0, 4.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };

	optional_vector result = aperture_ray_intersection(&aperture, ray);
	mu_assert("no aperture-ray intersection", result.present == true);

	return 0;
}

static char *
test_coincident()
{
	struct ray ray = { .position = { 0.0, 4.0, 1.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 1.0, 0.0, 0.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };

	optional_vector result = aperture_ray_intersection(&aperture, ray);
	mu_assert("no aperture-ray intersection", result.present == true);

	struct vector intersection = result.value;
	struct vector expected = ray.position;

	mu_assert("aperture-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_coincident_same_position()
{
	struct ray ray = { .position = { 0.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };

	optional_vector result = aperture_ray_intersection(&aperture, ray);
	mu_assert("aperture-ray intersection", result.present == false);

	return 0;
}

static char *
test_no_intersection()
{
	struct ray ray = { .position = { 6.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };

	optional_vector result = aperture_ray_intersection(&aperture, ray);
	mu_assert("aperture-ray intersection", result.present == false);

	return 0;
}

static char *
test_hit_normal()
{
	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };
	struct vector hit_position = { 4.0, 0.0, 1.0 };
	struct vector expected = { 0.0, 0.0, 1.0 };

	optional_vector result = aperture_hit_normal(&aperture, hit_position);
	mu_assert("no aperture hit normal", result.present == true);
	mu_assert("aperture hit normal not equal to expected",
	          vector_equals(result.value, expected));

	return 0;
}

static char *
test_hit_normal_no_hit()
{
	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };
	struct vector hit_position = { 1.0, 0.0, 1.0 };

	optional_vector result = aperture_hit_normal(&aperture, hit_position);
	mu_assert("unexpected aperture hit normal", result.present == false);

	return 0;
}

static char *
test_ray_entering()
{
	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };
	struct ray ray = { .position = { 4.0, 0.0, 0.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	bool result = aperture_ray_entering(&aperture, ray);
	mu_assert("ray entering aperture", result == false);

	return 0;
}

static char *
test_ray_leaving()
{
	struct aperture aperture = { .position = { 0.0, 0.0, 1.0 },
		                     .normal = { 0.0, 0.0, 1.0 },
		                     .r_opening = 3.0,
		                     .r_size = 5.0 };
	struct ray ray = { .position = { 4.0, 0.0, 1.0 },
		           .direction = { 0.0, 0.0, 1.0 } };

	bool result = aperture_ray_leaving(&aperture, ray);
	mu_assert("ray leaving aperture", result == false);

	return 0;
}

char *
test_aperture_all()
{
	// run all plane tests
	mu_run_test(test_intersection);
	mu_run_test(test_intersection_angled);
	mu_run_test(test_coincident);
	mu_run_test(test_coincident_same_position);
	mu_run_test(test_no_intersection);
	mu_run_test(test_hit_normal);
	mu_run_test(test_hit_normal_no_hit);
	mu_run_test(test_ray_entering);
	mu_run_test(test_ray_leaving);
	return 0;
}
