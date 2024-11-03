#include "minunit.h"

#include "engine/vector.h"
#include "tests/engine/vector.h"

static char* test_dot_product() {
    struct vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    struct vector v2 = {.x = 2.0f, .y = 3.0f, .z = 4.0f};

    float result = vector_dot(v1, v2);
    mu_assert("incorrect vector dot product", result == 20.0);

    return 0;
}

static char* test_add() {
    struct vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    struct vector v2 = {.x = 2.0f, .y = 3.0f, .z = 4.0f};

    struct vector result = vector_add(v1, v2);
    mu_assert("incorrect vector add .x", result.x == 3.0);
    mu_assert("incorrect vector add .y", result.y == 5.0);
    mu_assert("incorrect vector add .z", result.z == 7.0);

    return 0;
}

static char* test_subtract() {
    struct vector v1 = {.x = 1.0f, .y = 5.0f, .z = 0.0f};
    struct vector v2 = {.x = 2.0f, .y = 3.0f, .z = 0.0f};

    struct vector result = vector_subtract(v1, v2);
    mu_assert("incorrect vector subtract .x", result.x == -1.0);
    mu_assert("incorrect vector subtract .y", result.y == 2.0);
    mu_assert("incorrect vector subtract .z", result.z == 0.0);

    return 0;
}

static char* test_scalar_multiply() {
    struct vector v1 = {.x = 1.0f, .y = 2.0f, .z = 3.0f};
    float scalar = 5.0;

    struct vector result = vector_multiply(v1, scalar);
    mu_assert("incorrect vector scalar multiply .x", result.x == 5.0);
    mu_assert("incorrect vector scalar multiply .y", result.y == 10.0);
    mu_assert("incorrect vector scalar multiply .z", result.z == 15.0);

    return 0;
}

char* test_vector_all() {
    // run all vector tests
    mu_run_test(test_dot_product);
    mu_run_test(test_add);
    mu_run_test(test_subtract);
    mu_run_test(test_scalar_multiply);
    return 0;
}

// todo: test dot product