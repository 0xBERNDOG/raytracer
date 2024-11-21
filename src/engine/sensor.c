#include "engine/sensor.h"
#include "engine/hit_data.h"
#include "engine/image.h"
#include "engine/ray.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535
#endif

static double
shoot_rays(struct ray ray, struct object objects[], size_t num_objects,
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

	double reflected_brightness = 0.0;
	if (data.reflectivity.present) {
		// update ray position to be reflected by the
		// hit
		struct ray reflected_ray = {
			.direction = vector_reflect(ray.direction, data.normal),
			.position = data.position
		};

		reflected_brightness =
			shoot_rays(reflected_ray, objects, num_objects,
		                   depth - 1, data.object) *
			data.reflectivity.value;
	}

	double transmitted_brightness = 0.0;
	if (data.transmissivity.present) {
		bool refraction = true;

		// update ray position to be refracted by the
		// hit
		double n1 = 1.0;
		double n2 = 1.0;

		// todo: this doesnt work for objects that are touching, assumes
		// we go in/out of air all the time

		// check for entering/leaving from the midpoint between the shot
		// ray and the hit position to prevent spurious detections
		struct ray advanced_ray = ray;
		advanced_ray.position = vector_multiply(
			vector_add(data.position, ray.position), 0.5);

		if (data.object->func_ray_entering(data.object->object,
		                                   advanced_ray)) {
			n1 = 1.0;
			n2 = data.refractive_index.value;
		} else if (data.object->func_ray_leaving(data.object->object,
		                                         advanced_ray)) {
			n1 = data.refractive_index.value;
			n2 = 1.0;
		} else {
			// neither entering nor leaving the object, so no
			// optical boundary
			refraction = false;
		}

		optional_vector refracted_direction =
			vector_refract(ray.direction, data.normal, n1, n2);

		if (!refracted_direction.present) {
			refraction = false;
		}

		if (refraction) {
			struct ray refracted_ray = {
				.direction = refracted_direction.value,
				.position = data.position
			};

			transmitted_brightness =
				shoot_rays(refracted_ray, objects, num_objects,
			                   depth - 1, data.object) *
				data.transmissivity.value;
		}
	}

	double brightness =
		reflected_brightness + transmitted_brightness + data.brightness;

	return brightness;
}

static struct ray *
sensor_generate_uniformly_for_pixel(struct sensor_params sensor, size_t x,
                                    size_t y, double solid_angle, size_t n)
{
	// generate n^2 rays uniformly distributed across pixel (x,y) on the
	// sensor with a given solid angle (in degrees)
	struct ray *ray_array = malloc(n * n * sizeof(struct ray));
	if (!ray_array) {
		return NULL;
	}

	assert(n > 0);

	// generate a grid on the pixel's surface (ray positions)
	double width_per_sample = sensor.pixel_spacing / n;

	struct vector pixel_corner;
	struct vector pixel_midpoint;
	struct vector x_basis = { 1.0, 0.0, 0.0 };

	struct vector y_basis = { 0.0, 1.0, 0.0 };
	{
		// generate pixel positions in the sensor plane
		struct vector normal = { 0.0, 0.0, 1.0 };
		struct vector rotation_axis =
			vector_cross(normal, sensor.plane.normal);

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

		struct vector offset_midpoint_x = vector_multiply(
			x_basis, ((double)x - (sensor.width - 1) / 2.0) *
					 sensor.pixel_spacing);
		struct vector offset_midpoint_y = vector_multiply(
			y_basis, ((double)y - (sensor.height - 1) / 2.0) *
					 sensor.pixel_spacing);
		struct vector offset_midpoint =
			vector_add(offset_midpoint_x, offset_midpoint_y);

		struct vector offset_corner_x = vector_multiply(
			x_basis, ((double)x - 0.5 - (sensor.width - 1) / 2.0) *
					 sensor.pixel_spacing);
		struct vector offset_corner_y = vector_multiply(
			y_basis, ((double)y - 0.5 - (sensor.height - 1) / 2.0) *
					 sensor.pixel_spacing);
		struct vector offset_corner =
			vector_add(offset_corner_x, offset_corner_y);

		pixel_corner = vector_add(sensor.plane.position, offset_corner);
		pixel_midpoint =
			vector_add(sensor.plane.position, offset_midpoint);
	}

	// solid angle determines how far back to put the direction vector's
	// starting point
	double dist =
		sensor.pixel_spacing / (2 * tan(M_PI * solid_angle / 180.0));
	struct vector starting_position =
		vector_add(pixel_midpoint,
	                   vector_multiply(sensor.plane.normal, -1.0 * dist));

	// fill up the array with data
	for (size_t i = 0; i < n; i++) {
		struct vector offset_x = vector_multiply(
			x_basis, (i + 1) * (width_per_sample / 2.0));

		for (size_t j = 0; j < n; j++) {
			struct vector offset_y = vector_multiply(
				y_basis, (j + 1) * (width_per_sample / 2.0));
			struct vector offset = vector_add(offset_x, offset_y);

			ray_array[i * j + j].position =
				vector_add(pixel_corner, offset);
			ray_array[i * j + j].direction = vector_normalise(
				vector_subtract(ray_array[i * j + j].position,
			                        starting_position));
		}
	}

	return ray_array;
}

struct image *
sensor_capture(struct sensor_params params, struct object objects[],
               size_t num_objects)
{
	if (objects == NULL) {
		return NULL;
	}

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

	for (size_t x = 0; x < params.width; x++) {
		for (size_t y = 0; y < params.height; y++) {
			// generate rays for the pixel
			size_t n = 1;    // num_rays_per_pixel = n^2
			struct ray *rays = sensor_generate_uniformly_for_pixel(
				params, x, y, 25, n);

			double brightness = 0.0;
			for (size_t i = 0; i < n * n; i++) {
				brightness += shoot_rays(rays[i], objects,
				                         num_objects, 4, NULL);
			}
			brightness /= (n * n);

			if (brightness > 1.0) {
				brightness = 1.0;
			}

			size_t i = x * params.width + y;
			image->pixels[i].x = 255.0 * brightness;
			image->pixels[i].y = 255.0 * brightness;
			image->pixels[i].z = 255.0 * brightness;

			free(rays);
		}
	}

	return image;
}
