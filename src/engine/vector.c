#include "engine/vector.h"
#include <assert.h>
#include <math.h>

float vector_dot(vector v1, vector v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

vector vector_add(vector v1, vector v2) {
    vector result = {
        .x = (v1.x + v2.x), .y = (v1.y + v2.y), .z = (v1.z + v2.z)};
    return result;
}

vector vector_subtract(vector v1, vector v2) {
    return vector_add(v1, vector_multiply(v2, -1));
}

vector vector_multiply(vector v1, float scalar) {
    vector result = {
        .x = (v1.x * scalar), .y = (v1.y * scalar), .z = (v1.z * scalar)};
    return result;
}

bool vector_equals(vector v1, vector v2) {
    // todo: make fuzzy (so that floats can be equal if they're close)
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

float vector_len(vector v) { return sqrt(vector_dot(v, v)); }

vector vector_normalise(vector v) {
    float length = vector_len(v);
    assert(length > 0.0f);

    return vector_multiply(v, 1.0f / length);
}