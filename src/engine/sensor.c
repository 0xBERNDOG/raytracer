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
		// todo: shoot rays in random directions (smoothness)
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
		optional_ray refracted_ray =
			data.object->func_ray_refraction(ray, data);
		if (refracted_ray.present) {
			transmitted_brightness =
				shoot_rays(refracted_ray.value, objects,
			                   num_objects, depth - 1,
			                   data.object) *
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
		struct vector offset_x =
			vector_multiply(x_basis, (i + 0.5) * width_per_sample);

		for (size_t j = 0; j < n; j++) {
			struct vector offset_y = vector_multiply(
				y_basis, (j + 0.5) * width_per_sample);
			struct vector offset = vector_add(offset_x, offset_y);

			size_t index = (i * n) + j;
			ray_array[index].position =
				vector_add(pixel_corner, offset);
			ray_array[index].direction = vector_normalise(
				vector_subtract(ray_array[index].position,
			                        starting_position));

			ray_array[index].position = pixel_midpoint;
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

	double max_brightness = 1.0;
	for (size_t x = 0; x < params.width; x++) {
		for (size_t y = 0; y < params.height; y++) {
			// generate rays for the pixel
			size_t n = 20;    // num_rays_per_pixel = n^2
			struct ray *rays = sensor_generate_uniformly_for_pixel(
				params, x, y, 15,
				n);    // todo: figure out what value angle to
			               // use based on lens system?

			double brightness = 0.0;
			for (size_t i = 0; i < n * n; i++) {
				brightness += (1.0 / (n * n)) *
				              shoot_rays(rays[i], objects,
				                         num_objects, 5, NULL);
			}

			// simple exposure adjust (prevents max value from
			// clipping)
			if (brightness > max_brightness) {
				max_brightness = brightness;
			}

			size_t i = x * params.width + y;
			image->pixels[i].x = 255.0 * brightness;
			image->pixels[i].y = 255.0 * brightness;
			image->pixels[i].z = 255.0 * brightness;

			free(rays);
		}
	}

	// simple exposure adjust (prevents max value from clipping)
	for (size_t x = 0; x < params.width; x++) {
		for (size_t y = 0; y < params.height; y++) {
			size_t i = x * params.width + y;

			image->pixels[i].x =
				image->pixels[i].x / max_brightness;
			image->pixels[i].y =
				image->pixels[i].y / max_brightness;
			image->pixels[i].z =
				image->pixels[i].z / max_brightness;
		}
	}

	return image;
}
