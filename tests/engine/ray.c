#include "engine/ray.h"

#include "engine/object.h"
#include "engine/plane.h"
#include "minunit.h"
#include "tests/engine/ray.h"

static char *
test_intersection()
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

	struct object front_object = create_plane(&front_plane);
	struct object rear_object = create_plane(&rear_plane);

	world[0] = front_object;
	world[1] = rear_object;

	optional_hit_data result = ray_get_hit_data(&ray, world, 2);
	mu_assert("no plane-ray intersection", result.present == true);

	struct vector intersection = result.value.position;

	struct vector expected = { .x = 5.0, .y = 0.0, .z = 0.0 };
	mu_assert("plane-ray intersection not equal to expected",
	          vector_equals(intersection, expected));

	return 0;
}

char *
test_ray_all()
{
	// run all ray tests
	mu_run_test(test_intersection);
	return 0;
}
