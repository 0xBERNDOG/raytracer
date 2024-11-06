#include "engine/vector.h"

#include <math.h>

#include "minunit.h"
#include "tests/engine/vector.h"
#include "utils/compare.h"

static char *
test_equals()
{
	struct vector v1 = { .x = 1.0, .y = 2.0, .z = 3.0 };
	struct vector v2 = { .x = 1.0, .y = 2.0, .z = 3.0 };

	bool result = vector_equals(v1, v2);
	mu_assert("vectors not equal", result == true);

	return 0;
}

static char *
test_dot_product()
{
	struct vector v1 = { .x = 1.0, .y = 2.0, .z = 3.0 };
	struct vector v2 = { .x = 2.0, .y = 3.0, .z = 4.0 };

	double result = vector_dot(v1, v2);
	mu_assert("incorrect vector dot product", result == 20.0);

	return 0;
}

static char *
test_add()
{
	struct vector v1 = { .x = 1.0, .y = 2.0, .z = 3.0 };
	struct vector v2 = { .x = 2.0, .y = 3.0, .z = 4.0 };

	struct vector result = vector_add(v1, v2);
	struct vector expected = { .x = 3.0, .y = 5.0, .z = 7.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_subtract()
{
	struct vector v1 = { .x = 1.0, .y = 5.0, .z = 0.0 };
	struct vector v2 = { .x = 2.0, .y = 3.0, .z = 0.0 };

	struct vector result = vector_subtract(v1, v2);
	struct vector expected = { .x = -1.0, .y = 2.0, .z = 0.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_scalar_multiply()
{
	struct vector v1 = { .x = 1.0, .y = 2.0, .z = 3.0 };
	double scalar = 5.0;

	struct vector result = vector_multiply(v1, scalar);
	struct vector expected = { .x = 5.0, .y = 10.0, .z = 15.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_length()
{
	struct vector v = { .x = 1.0, .y = 2.0, .z = 3.0 };

	double expected = sqrt(1 + 4 + 9);
	double result = vector_len(v);
	mu_assert("length not equal to expected", result == expected);

	return 0;
}

static char *
test_normalise()
{
	struct vector v = { .x = 3.0, .y = 4.0, .z = 5.0 };
	struct vector result = vector_normalise(v);

	double length = vector_len(result);
	mu_assert("length not equal to 1.0", FUZZY_EQUALS(length, 1.0));

	return 0;
}

char *
test_vector_all()
{
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
