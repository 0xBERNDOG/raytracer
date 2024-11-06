#include "engine/ray.h"

#include "engine/object.h"
#include "engine/plane.h"
#include "minunit.h"
#include "tests/engine/ray.h"

static char *
test_nearest_intersection()
{
	struct object world[2];

	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = { .x = 1.0, .y = 0.0, .z = 0.0 } };

	struct plane front_plane = {
		.position = { .x = 5.0, .y = 0.0, .z = 0.0 },
		.normal = { .x = 1.0, .y = 0.0, .z = 0.0 }
	};

	struct plane rear_plane = {
		.position = { .x = 10.0, .y = 0.0, .z = 0.0 },
		.normal = { .x = 1.0, .y = 0.0, .z = 0.0 }
	};

	struct object front_object = { .object = (void *)&front_plane,
		                       .func_ray_intersection =
		                               &plane_ray_intersection };

	struct object rear_object = { .object = (void *)&rear_plane,
		                      .func_ray_intersection =
		                              &plane_ray_intersection };

	world[0] = front_object;
	world[1] = rear_object;

	optional_vector result = ray_get_nearest_hit_position(&ray, world, 2);
	mu_assert("no plane-ray intersection", result.present == true);

	struct vector intersection = result.value;

	struct vector expected = { .x = 5.0, .y = 0.0, .z = 0.0 };
	mu_assert("plane-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

char *
test_ray_all()
{
	// run all ray tests
	mu_run_test(test_nearest_intersection);
	return 0;
}
