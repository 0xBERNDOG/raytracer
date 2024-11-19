#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

#include "utils/optional.h"

struct vector {
	double x;
	double y;
	double z;
};

typedef optional_type(struct vector) optional_vector;

double vector_dot(struct vector v1, struct vector v2);
double vector_len(struct vector v);

struct vector vector_cross(struct vector v1, struct vector v2);
struct vector vector_add(struct vector v1, struct vector v2);
struct vector vector_subtract(struct vector v1, struct vector v2);
struct vector vector_multiply(struct vector v1, double scalar);
struct vector vector_normalise(struct vector v);
struct vector vector_reflect(struct vector v, struct vector normal);
optional_vector vector_refract(struct vector v, struct vector normal, double n1,
                               double n2);

bool vector_equals(struct vector v1, struct vector v2);

#endif
