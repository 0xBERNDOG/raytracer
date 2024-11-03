#include "minunit.h"

#include "tests/engine/main.h"
#include "tests/engine/sphere.h"

char* test_engine_all() {
    // run all engine tests
    mu_run_test_group(test_sphere_all);
    return 0;
}
