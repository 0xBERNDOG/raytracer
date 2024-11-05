#include "engine/vector.h"
#include <assert.h>
#include <math.h>

double vector_dot(struct vector v1, struct vector v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

struct vector vector_add(struct vector v1, struct vector v2) {
    struct vector result = {
        .x = (v1.x + v2.x), .y = (v1.y + v2.y), .z = (v1.z + v2.z)};
    return result;
}

struct vector vector_subtract(struct vector v1, struct vector v2) {
    return vector_add(v1, vector_multiply(v2, -1));
}

struct vector vector_multiply(struct vector v1, double scalar) {
    struct vector result = {
        .x = (v1.x * scalar), .y = (v1.y * scalar), .z = (v1.z * scalar)};
    return result;
}

bool vector_equals(struct vector v1, struct vector v2) {
    // fuzzy equals
    double tolerance = 0.00001;
    return ((fabs(v1.x - v2.x) <= tolerance) &&
            (fabs(v1.y - v2.y) <= tolerance) &&
            (fabs(v1.z - v2.z) <= tolerance));
}

double vector_len(struct vector v) { return sqrt(vector_dot(v, v)); }

struct vector vector_normalise(struct vector v) {
    double length = vector_len(v);
    assert(length > 0.0);

    return vector_multiply(v, 1.0 / length);
}