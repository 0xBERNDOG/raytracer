#ifndef VECTOR_H
#define VECTOR_H

#include "utils/optional.h"
#include <stdbool.h>

struct vector {
    double x;
    double y;
    double z;
};

typedef optional_type(struct vector) optional_vector;

double vector_dot(struct vector v1, struct vector v2);
double vector_len(struct vector v);

struct vector vector_add(struct vector v1, struct vector v2);
struct vector vector_subtract(struct vector v1, struct vector v2);
struct vector vector_multiply(struct vector v1, double scalar);
struct vector vector_normalise(struct vector v);

bool vector_equals(struct vector v1, struct vector v2);

#endif
