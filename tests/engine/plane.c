#include "minunit.h"

#include "engine/plane.h"
#include "engine/ray.h"
#include "tests/engine/plane.h"

static char* test_intersection() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    struct plane plane = {.position = {.x = 0.0f, .y = 0.0f, .z = 1.0f},
                          .normal = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    optional_vector result = plane_ray_intersection(&plane, &ray);
    mu_assert("no plane-ray intersection", result.present == true);

    struct vector intersection = result.value;

    struct vector expected = {.x = 0.0f, .y = 0.0f, .z = 1.0f};
    mu_assert("plane-ray intersection not equal to expected",
              vector_equals(intersection, expected));

    return 0;
}

static char* test_intersection_angled() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 0.0f, .y = 1.0f, .z = 1.0f}};

    struct plane plane = {.position = {.x = 0.0f, .y = 0.0f, .z = 1.0f},
                          .normal = {.x = 1.0f, .y = 1.0f, .z = 1.0f}};

    optional_vector result = plane_ray_intersection(&plane, &ray);
    mu_assert("no plane-ray intersection", result.present == true);

    return 0;
}

static char* test_coincident() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 1.0f},
                      .direction = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    struct plane plane = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                          .normal = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    optional_vector result = plane_ray_intersection(&plane, &ray);
    mu_assert("no plane-ray intersection", result.present == true);

    struct vector intersection = result.value;
    struct vector expected = ray.position;

    mu_assert("plane-ray intersection not equal to expected",
              vector_equals(intersection, expected));

    return 0;
}

static char* test_coincident_same_position() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    struct plane plane = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                          .normal = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    optional_vector result = plane_ray_intersection(&plane, &ray);
    mu_assert("no plane-ray intersection", result.present == true);

    struct vector intersection = result.value;
    struct vector expected = ray.position;

    mu_assert("plane-ray intersection not equal to expected",
              vector_equals(intersection, expected));

    return 0;
}

static char* test_no_intersection() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    struct plane plane = {.position = {.x = 1.0f, .y = 0.0f, .z = 0.0f},
                          .normal = {.x = 1.0f, .y = 0.0f, .z = 0.0f}};

    optional_vector result = plane_ray_intersection(&plane, &ray);
    mu_assert("plane-ray intersection", result.present == false);

    return 0;
}

char* test_plane_all() {
    // run all plane tests
    mu_run_test(test_intersection);
    mu_run_test(test_intersection_angled);
    mu_run_test(test_coincident);
    mu_run_test(test_coincident_same_position);
    mu_run_test(test_no_intersection);
    return 0;
}
