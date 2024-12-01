#include "engine/object.h"

optional_ray
object_default_refraction(struct ray ray, struct hit_data data)
{
	// todo: shoot rays in random directions (transparency)
	optional_ray result = { .present = false };

	// update ray position to be refracted by the
	// hit
	double n1 = 1.0;
	double n2 = 1.0;

	// todo: this doesnt work for objects that are touching, assumes
	// we go in/out of air all the time

	// check for entering/leaving from the midpoint between the shot
	// ray and the hit position to prevent spurious detections
	struct ray advanced_ray = ray;
	advanced_ray.position =
		vector_multiply(vector_add(data.position, ray.position), 0.5);

	if (data.object->func_ray_entering(data.object->object, advanced_ray)) {
		n1 = 1.0;
		n2 = data.refractive_index.value;
	} else if (data.object->func_ray_leaving(data.object->object,
	                                         advanced_ray)) {
		n1 = data.refractive_index.value;
		n2 = 1.0;
	} else {
		// neither entering nor leaving the object, so no
		// optical boundary
		return result;
	}

	optional_vector refracted_direction =
		vector_refract(ray.direction, data.normal, n1, n2);

	if (refracted_direction.present) {
		result.present = true;
		result.value.direction = refracted_direction.value;
		result.value.position = data.position;
	}

	return result;
}