/*
    MinUnit
    https://jera.com/techinfo/jtns/jtn002
*/

#define mu_assert(message, test)     \
    do {                             \
        if (!(test)) return message; \
    } while (0)

#define mu_run_test(test)            \
    do {                             \
        char *message = test();      \
        tests_run++;                 \
        if (message) return message; \
    } while (0)

// a test group is a test that just doesn't increment the test counter, since
// it's just a grouping of tests
#define mu_run_test_group(test)      \
    do {                             \
        char *message = test();      \
        if (message) return message; \
    } while (0)

extern int tests_run;
