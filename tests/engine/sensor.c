#include "engine/sensor.h"

#include "minunit.h"
#include "tests/engine/sensor.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

static char *
test_no_scene()
{
	struct sensor_params params = { .width = 10,
		                        .height = 10,
		                        .pixel_spacing = 1.0 };

	struct vector *result = sensor_capture(params, NULL, 0);
	mu_assert("unexpected data returned by sensor", result == NULL);

	return 0;
}

static char *
test_empty_scene()
{
	struct object *world = calloc(10, sizeof(struct object));
	mu_assert("failed to allocate memory for world", world != NULL);

	struct sensor_params params = {
		.plane = { .position = { 0.0, 0.0, 0.0 },
		           .normal = { 0.0, 0.0, 1.0 } },
		.width = 10,
		.height = 10,
		.pixel_spacing = 1.0
	};

	struct vector expected[100];
	for (size_t i = 0; i < 100; i++) {
		expected[i].x = 0.0;
		expected[i].y = 0.0;
		expected[i].z = 0.0;
	}

	struct vector *result = sensor_capture(params, world, 0);
	mu_assert("no data returned by sensor", result != NULL);

	for (size_t i = 0; i < 100; i++) {
		mu_assert("sensor data doesn't match expected",
		          vector_equals(expected[i], result[i]));
	}

	free(result);

	return 0;
}

static char *
test_basic_scene()
{
	struct object *world = calloc(10, sizeof(struct object));
	mu_assert("failed to allocate memory for world", world != NULL);

	struct sensor_params params = {
		.plane = { .position = { 0.0, 0.0, 0.0 },
		           .normal = { 1.0, 0.0, 0.0 } },
		.width = 10,
		.height = 10,
		.pixel_spacing = 1.0
	};

	struct plane plane = { .normal = { 1.0, 0.0, 0.0 },
		               .position = { 10.0, 0.0, 0.0 } };
	world[0] = create_plane(&plane);

	struct vector *result = sensor_capture(params, world, 1);
	mu_assert("no data returned by sensor", result != NULL);

	for (size_t i = 0; i < 100; i++) {
		mu_assert("sensor data doesn't match expected",
		          vector_len(result[i]) > 0.0);
	}

	free(result);

	return 0;
}

static char *
test_no_hits()
{
	struct object *world = calloc(10, sizeof(struct object));
	mu_assert("failed to allocate memory for world", world != NULL);

	struct sensor_params params = {
		.plane = { .position = { 0.0, 0.0, 0.0 },
		           .normal = { 1.0, 0.0, 0.0 } },
		.width = 10,
		.height = 10,
		.pixel_spacing = 1.0
	};

	struct plane plane = { .normal = { 0.0, 0.0, 1.0 },
		               .position = { 0.0, 0.0, 20.0 } };
	world[0] = create_plane(&plane);

	struct vector expected[100];
	for (size_t i = 0; i < 100; i++) {
		expected[i].x = 0.0;
		expected[i].y = 0.0;
		expected[i].z = 0.0;
	}

	struct vector *result = sensor_capture(params, world, 0);
	mu_assert("no data returned by sensor", result != NULL);

	for (size_t i = 0; i < 100; i++) {
		mu_assert("sensor data doesn't match expected",
		          vector_equals(expected[i], result[i]));
	}

	free(result);

	return 0;
}

char *
test_sensor_all()
{
	// run all sensor tests
	mu_run_test(test_no_scene);
	mu_run_test(test_empty_scene);
	mu_run_test(test_no_hits);
	mu_run_test(test_basic_scene);
	return 0;
}