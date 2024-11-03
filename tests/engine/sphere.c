#include "minunit.h"

#include "tests/engine/sphere.h"

char* test_sphere_all() {
    // run all sphere tests
    mu_run_test(test_sphere_example);
    return 0;
}

char* test_sphere_example() { return 0; }

// todo: test sphere-ray intersection