#include "minunit.h"

#include <math.h>

#include "engine/vector.h"
#include "tests/engine/vector.h"

static char* test_equals() {
    // todo: test nearby floats
    vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector v2 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    bool result = vector_equals(v1, v2);
    mu_assert("vectors not equal", result == true);

    return 0;
}

static char* test_dot_product() {
    vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector v2 = {.x = 2.0f, .y = 3.0f, .z = 4.0f};

    float result = vector_dot(v1, v2);
    mu_assert("incorrect vector dot product", result == 20.0);

    return 0;
}

static char* test_add() {
    vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    vector v2 = {.x = 2.0f, .y = 3.0f, .z = 4.0f};

    vector result = vector_add(v1, v2);
    vector expected = {.x = 3.0f, .y = 5.0f, .z = 7.0f};
    mu_assert("vector not equal to expected", vector_equals(result, expected));

    return 0;
}

static char* test_subtract() {
    vector v1 = {.x = 1.0f, .y = 5.0f, .z = 0.0f};
    vector v2 = {.x = 2.0f, .y = 3.0f, .z = 0.0f};

    vector result = vector_subtract(v1, v2);
    vector expected = {.x = -1.0f, .y = 2.0f, .z = 0.0f};
    mu_assert("vector not equal to expected", vector_equals(result, expected));

    return 0;
}

static char* test_scalar_multiply() {
    vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    float scalar = 5.0;

    vector result = vector_multiply(v1, scalar);
    vector expected = {.x = 5.0f, .y = 10.0f, .z = 15.0f};
    mu_assert("vector not equal to expected", vector_equals(result, expected));

    return 0;
}

static char* test_length() {
    vector v = {.x = 1.0f, .y = 2.0f, .z = 3.0f};

    float expected = sqrt(1 + 4 + 9);
    float result = vector_len(v);
    mu_assert("length not equal to expected", result == expected);

    return 0;
}

static char* test_normalise() {
    vector v = {.x = 3.0f, .y = 4.0f, .z = 5.0f};
    vector result = vector_normalise(v);

    // todo: fuzzy equals for floats
    float tolerance = 0.00001f;
    float length = vector_len(result);
    mu_assert("length not equal to 1.0",
              (length > 1.0f - tolerance) && (length < 1.0f + tolerance));

    return 0;
}

char* test_vector_all() {
    // run all vector tests
    mu_run_test(test_equals);
    mu_run_test(test_dot_product);
    mu_run_test(test_add);
    mu_run_test(test_subtract);
    mu_run_test(test_scalar_multiply);
    mu_run_test(test_length);
    mu_run_test(test_normalise);
    return 0;
}
