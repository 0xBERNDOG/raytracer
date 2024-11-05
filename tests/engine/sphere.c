#include "minunit.h"

#include "engine/ray.h"
#include "engine/sphere.h"
#include "tests/engine/sphere.h"

#include <math.h>

static char* test_intersection() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    struct sphere sphere = {.position = {.x = 5.0f, .y = 0.0f, .z = 0.0f},
                            .radius = 1.0f};

    optional_vector result = sphere_ray_intersection(&sphere, &ray);
    mu_assert("no sphere-ray intersection", result.present == true);

    struct vector intersection = result.value;

    struct vector expected = {.x = 4.0f, .y = 0.0f, .z = 0.0f};
    mu_assert("sphere-ray intersection not equal to expected",
              vector_equals(intersection, expected));

    return 0;
}

static char* test_offset() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.5f},
                      .direction = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    struct sphere sphere = {.position = {.x = 5.0f, .y = 0.0f, .z = 0.0f},
                            .radius = 1.0f};

    optional_vector result = sphere_ray_intersection(&sphere, &ray);
    mu_assert("no sphere-ray intersection", result.present == true);

    struct vector intersection = result.value;
    struct vector diff = vector_subtract(sphere.position, intersection);
    float expected = 1.0f;
    float tolerance = 0.0001f;

    mu_assert("sphere-ray intersection distance not equal to expected",
              fabs(vector_len(diff) - expected) < tolerance);

    return 0;
}

static char* test_intersection_glancing() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 1.0f},
                      .direction = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    struct sphere sphere = {.position = {.x = 5.0f, .y = 0.0f, .z = 0.0f},
                            .radius = 1.0f};

    optional_vector result = sphere_ray_intersection(&sphere, &ray);
    mu_assert("no sphere-ray intersection", result.present == true);

    struct vector intersection = result.value;

    struct vector expected = {.x = 5.0f, .y = 0.0f, .z = 1.0f};
    mu_assert("sphere-ray intersection not equal to expected",
              vector_equals(intersection, expected));

    return 0;
}

static char* test_intersection_from_inside() {
    struct ray ray = {.position = {.x = 4.5f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    struct sphere sphere = {.position = {.x = 5.0f, .y = 0.0f, .z = 0.0f},
                            .radius = 1.0f};

    optional_vector result = sphere_ray_intersection(&sphere, &ray);
    mu_assert("no sphere-ray intersection", result.present == true);

    struct vector intersection = result.value;

    struct vector expected = {.x = 6.0f, .y = 0.0f, .z = 0.0f};
    mu_assert("sphere-ray intersection not equal to expected",
              vector_equals(intersection, expected));

    return 0;
}

static char* test_no_intersection() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 5.0f},
                      .direction = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    struct sphere sphere = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                            .radius = 1.0f};

    optional_vector result = sphere_ray_intersection(&sphere, &ray);
    mu_assert("sphere-ray intersection", result.present == false);

    return 0;
}

char* test_sphere_all() {
    // run all sphere tests
    mu_run_test(test_intersection);
    mu_run_test(test_offset);
    mu_run_test(test_intersection_glancing);
    mu_run_test(test_intersection_from_inside);
    mu_run_test(test_no_intersection);
    return 0;
}
