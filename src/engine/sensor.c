#include "engine/sensor.h"
#include "engine/ray.h"

#include <stddef.h>
#include <stdlib.h>

struct vector *
sensor_capture(struct sensor_params params, struct object objects[],
               size_t num_objects)
{
	if (objects == NULL) {
		return NULL;
	}

	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = params.plane.normal };

	struct vector *pixels =
		calloc(params.width * params.height, sizeof(struct vector));

	// transform the ray's basis vectors (x,y) onto the
	// surface of the sensor's plane
	struct vector normal = { 0.0, 0.0, 1.0 };
	struct vector rotation_axis = vector_cross(normal, params.plane.normal);

	struct vector x_basis = { 1.0, 0.0, 0.0 };
	struct vector y_basis = { 0.0, 1.0, 0.0 };
	if (vector_len(rotation_axis) > 0.0) {
		struct vector x_basis_rotated =
			vector_cross(rotation_axis, x_basis);
		struct vector y_basis_rotated =
			vector_cross(rotation_axis, y_basis);

		// if rotated vector is zero, then it was
		// parallel to rotation axis (nothing to be
		// done)
		if (vector_len(x_basis_rotated) > 0.0) {
			x_basis = vector_normalise(x_basis_rotated);
		}

		if (vector_len(y_basis_rotated) > 0.0) {
			y_basis = vector_normalise(y_basis_rotated);
		}
	}

	// todo: replace this with proper hit data
	for (size_t x = 0; x < params.width; x++) {
		for (size_t y = 0; y < params.height; y++) {
			struct vector offset_x = vector_multiply(
				x_basis, (double)x * params.pixel_spacing);
			struct vector offset_y = vector_multiply(
				y_basis, (double)y * params.pixel_spacing);
			struct vector offset = vector_add(offset_x, offset_y);
			ray.position =
				vector_add(params.plane.position, offset);

			// todo: update to use proper hit data
			// (just gives a white pixel for a hit)
			optional_vector hit_position =
				ray_get_nearest_hit_position(&ray, objects,
			                                     num_objects);
			if (hit_position.present) {
				size_t i = x * params.width + y;
				pixels[i].x = 255;
				pixels[i].y = 255;
				pixels[i].z = 255;
			}
		}
	}

	return pixels;
}