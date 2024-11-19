#include "engine/plane.h"

#include "engine/ray.h"
#include "minunit.h"
#include "tests/engine/plane.h"

static char *
test_intersection()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 1.0 },
		               .normal = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	optional_vector result = plane_ray_intersection(&plane, ray);
	mu_assert("no plane-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 0.0, .y = 0.0, .z = 1.0 };
	mu_assert("plane-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_intersection_angled()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 0.0, .y = 1.0, .z = 1.0 } };

	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 1.0 },
		               .normal = { .x = 1.0, .y = 1.0, .z = 1.0 } };

	optional_vector result = plane_ray_intersection(&plane, ray);
	mu_assert("no plane-ray intersection", result.present == true);

	return 0;
}

static char *
test_coincident()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 1.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		               .normal = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	optional_vector result = plane_ray_intersection(&plane, ray);
	mu_assert("no plane-ray intersection", result.present == true);

	struct vector intersection = result.value;
	struct vector expected = ray.position;

	mu_assert("plane-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_coincident_same_position()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		               .normal = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	optional_vector result = plane_ray_intersection(&plane, ray);
	mu_assert("no plane-ray intersection", result.present == true);

	struct vector intersection = result.value;
	struct vector expected = ray.position;

	mu_assert("plane-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

static char *
test_no_intersection()
{
	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 0.0, .y = 0.0, .z = 1.0 } };

	struct plane plane = { .position = { .x = 1.0, .y = 0.0, .z = 0.0 },
		               .normal = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	optional_vector result = plane_ray_intersection(&plane, ray);
	mu_assert("plane-ray intersection", result.present == false);

	return 0;
}

static char *
test_hit_normal()
{
	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		               .normal = { 0.0, 0.0, 1.0 } };
	struct vector hit_position = { 1.0, 0.0, 0.0 };
	struct vector expected = { 0.0, 0.0, 1.0 };

	optional_vector result = plane_hit_normal(&plane, hit_position);
	mu_assert("no plane hit normal", result.present == true);
	mu_assert("plane hit normal not equal to expected",
	          vector_equals(result.value, expected));

	return 0;
}

static char *
test_hit_normal_no_hit()
{
	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		               .normal = { 0.0, 0.0, 1.0 } };
	struct vector hit_position = { 1.0, 0.0, 1.0 };

	optional_vector result = plane_hit_normal(&plane, hit_position);
	mu_assert("unexpected plane hit normal", result.present == false);

	return 0;
}

static char *
test_ray_entering()
{
	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		               .normal = { 1.0, 0.0, 0.0 } };
	struct ray ray = { .position = { 1.0, 0.0, 0.0 },
		           .direction = { 1.0, 0.0, 0.0 } };

	bool result = plane_ray_entering(&plane, ray);
	mu_assert("ray entering plane", result == false);

	return 0;
}

static char *
test_ray_leaving()
{
	struct plane plane = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		               .normal = { 1.0, 0.0, 0.0 } };
	struct ray ray = { .position = { 1.0, 0.0, 0.0 },
		           .direction = { 1.0, 0.0, 0.0 } };

	bool result = plane_ray_leaving(&plane, ray);
	mu_assert("ray leaving plane", result == false);

	return 0;
}

char *
test_plane_all()
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
