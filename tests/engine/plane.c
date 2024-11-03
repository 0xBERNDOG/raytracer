#include "minunit.h"

#include "engine/plane.h"
#include "engine/ray.h"
#include "tests/engine/plane.h"

static char* test_plane_ray_intersection() {
    struct ray ray = {.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
                      .direction = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    struct plane plane = {.position = {.x = 0.0f, .y = 0.0f, .z = 1.0f},
                          .normal = {.x = 0.0f, .y = 0.0f, .z = 1.0f}};

    optional_vector result = plane_ray_intersection(&plane, &ray);

    mu_assert("no plane-ray intersection result", result.present);

    struct vector intersection = result.value;

    // todo: vector "nearly equal" function for floats
    mu_assert("incorrect plane-ray intersection.x", intersection.x == 0.0f);
    mu_assert("incorrect plane-ray intersection.y", intersection.y == 0.0f);
    mu_assert("incorrect plane-ray intersection.z", intersection.z == 1.0f);

    return 0;
}

static char* test_plane_ray_intersection_coincident() {
    // todo
    return 0;
}

static char* test_plane_ray_no_intersection() {
    // todo
    return 0;
}

char* test_plane_all() {
    // run all plane tests
    mu_run_test(test_plane_ray_intersection);
    mu_run_test(test_plane_ray_intersection_coincident);
    mu_run_test(test_plane_ray_no_intersection);
    return 0;
}

// todo: test plane-ray intersection