#include "minunit.h"

#include "tests/engine/main.h"
#include "tests/engine/plane.h"
#include "tests/engine/sphere.h"
#include "tests/engine/vector.h"

char* test_engine_all() {
    // run all engine tests
    mu_run_test_group(test_plane_all);
    mu_run_test_group(test_sphere_all);
    mu_run_test_group(test_vector_all);
    return 0;
}
