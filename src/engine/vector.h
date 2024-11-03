#ifndef VECTOR_H
#define VECTOR_H

#include "optional.h"

struct vector {
    float x;
    float y;
    float z;
};

typedef optional_type(struct vector) optional_vector;

#endif
