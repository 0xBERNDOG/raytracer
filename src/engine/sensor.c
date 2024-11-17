#include "engine/sensor.h"
#include "engine/hit_data.h"
#include "engine/image.h"
#include "engine/ray.h"

#include <stddef.h>
#include <stdlib.h>

static double
shoot_rays(struct ray *ray, struct object objects[], size_t num_objects,
           size_t depth, struct object *last_hit)
{
	if (depth == 0) {
		return 0.0;
	}

	// todo: update to use proper hit data
	// (just gives a white pixel for a hit)
	optional_hit_data hit_data =
		ray_get_hit_data(ray, objects, num_objects, last_hit);

	if (!hit_data.present) {
		return 0.0;
	}

	struct hit_data data = hit_data.value;

	// update ray position to be reflected by the
	// hit
	ray->direction = vector_reflect(ray->direction, data.normal);
	ray->position = data.position;

	double reflected_brightness =
		shoot_rays(ray, objects, num_objects, --depth, data.object) *
		data.reflectivity;
	double brightness = reflected_brightness + data.brightness;

	return brightness;
}

struct image *
sensor_capture(struct sensor_params params, struct object objects[],
               size_t num_objects)
{
	if (objects == NULL) {
		return NULL;
	}

	struct ray ray = { .position = { .x = 0.0, .y = 0.0, .z = 0.0 },
		           .direction = params.plane.normal };

	struct image *image = image_allocate(params.width, params.height);

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
				x_basis,
				((double)x - (params.width - 1) / 2.0) *
					params.pixel_spacing);
			struct vector offset_y = vector_multiply(
				y_basis,
				((double)y - (params.height - 1) / 2.0) *
					params.pixel_spacing);
			struct vector offset = vector_add(offset_x, offset_y);
			ray.position =
				vector_add(params.plane.position, offset);
			ray.direction = params.plane.normal;

			double brightness =
				shoot_rays(&ray, objects, num_objects, 3, NULL);

			if (brightness > 1.0) {
				brightness = 1.0;
			}

			size_t i = x * params.width + y;
			image->pixels[i].x = 255.0 * brightness;
			image->pixels[i].y = 255.0 * brightness;
			image->pixels[i].z = 255.0 * brightness;
		}
	}

	return image;
}