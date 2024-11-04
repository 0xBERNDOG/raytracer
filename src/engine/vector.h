#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include "optional.h"

struct vector {
    float x;
    float y;
    float z;
};

typedef optional_type(struct vector) optional_vector;

float vector_dot(struct vector v1, struct vector v2);
float vector_len(struct vector v);

struct vector vector_add(struct vector v1, struct vector v2);
struct vector vector_subtract(struct vector v1, struct vector v2);
struct vector vector_multiply(struct vector v1, float scalar);
struct vector vector_normalise(struct vector v);

bool vector_equals(struct vector v1, struct vector v2);

#endif
