#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include "optional.h"

typedef struct {
    float x;
    float y;
    float z;
} vector;

typedef optional_type(vector) optional_vector;

float vector_dot(vector v1, vector v2);
float vector_len(vector v);

vector vector_add(vector v1, vector v2);
vector vector_subtract(vector v1, vector v2);
vector vector_multiply(vector v1, float scalar);
vector vector_normalise(vector v);

bool vector_equals(vector v1, vector v2);

#endif
