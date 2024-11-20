#include "engine/vector.h"

#include <assert.h>
#include <math.h>

#include "utils/compare.h"

double
vector_dot(struct vector v1, struct vector v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

struct vector
vector_cross(struct vector v1, struct vector v2)
{
	// https://en.wikipedia.org/wiki/Cross_product
	struct vector result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

struct vector
vector_add(struct vector v1, struct vector v2)
{
	struct vector result = { .x = (v1.x + v2.x),
		                 .y = (v1.y + v2.y),
		                 .z = (v1.z + v2.z) };
	return result;
}

struct vector
vector_subtract(struct vector v1, struct vector v2)
{
	return vector_add(v1, vector_multiply(v2, -1));
}

struct vector
vector_multiply(struct vector v1, double scalar)
{
	struct vector result = { .x = (v1.x * scalar),
		                 .y = (v1.y * scalar),
		                 .z = (v1.z * scalar) };
	return result;
}

bool
vector_equals(struct vector v1, struct vector v2)
{
	// fuzzy equals
	return (FUZZY_EQUALS(v1.x, v2.x) && FUZZY_EQUALS(v1.y, v2.y) &&
	        FUZZY_EQUALS(v1.z, v2.z));
}

double
vector_len(struct vector v)
{
	return sqrt(vector_dot(v, v));
}

struct vector
vector_normalise(struct vector v)
{
	double length = vector_len(v);
	assert(length > 0.0);

	return vector_multiply(v, 1.0 / length);
}

struct vector
vector_reflect(struct vector v, struct vector _normal)
{
	// https://math.stackexchange.com/a/13263
	struct vector normal = vector_normalise(_normal);
	double dot = vector_dot(v, normal);

	struct vector result =
		vector_subtract(v, vector_multiply(normal, 2 * dot));
	return result;
}

optional_vector
vector_refract(struct vector v, struct vector _normal, double n1, double n2)
{
	// https://stackoverflow.com/a/58676386
	optional_vector result = { .present = false };

	struct vector normal = vector_normalise(_normal);

	// need normal vector to be pointing in the opposite direction to v
	double dot = vector_dot(v, normal);
	if (dot > 0.0) {
		normal = vector_multiply(normal, -1.0);
	}

	double n = n1 / n2;

	double cosI = -vector_dot(normal, v);
	double sinT2 = pow(n, 2) * (1.0 - pow(cosI, 2));

	if (sinT2 > 1.0) {
		// total internal reflection
		return result;
	}

	double cosT = sqrt(1.0 - sinT2);

	result.value = vector_add(vector_multiply(v, n),
	                          vector_multiply(normal, (n * cosI) - cosT));
	result.present = true;

	return result;
}