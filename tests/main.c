#include <stdio.h>
#include "minunit.h"

#include "tests/engine/main.h"

int tests_run = 0;

static char *test_example() {
    int bar = 5;
    mu_assert("error, bar != 5", bar == 5);
    return 0;
}

static char *tests_all() {
    mu_run_test(test_example);
    mu_run_test_group(test_engine_all);
    return 0;
}

int main() {
    char *result = tests_all();

    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
