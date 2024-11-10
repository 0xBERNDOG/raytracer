#include "engine/vector.h"

#include <math.h>

#include "minunit.h"
#include "tests/engine/vector.h"
#include "utils/compare.h"

static char *
test_equals()
{
	struct vector v1 = { 1.0, 2.0, 3.0 };
	struct vector v2 = { 1.0, 2.0, 3.0 };

	bool result = vector_equals(v1, v2);
	mu_assert("vectors not equal", result == true);

	return 0;
}

static char *
test_dot_product()
{
	struct vector v1 = { 1.0, 2.0, 3.0 };
	struct vector v2 = { 2.0, 3.0, 4.0 };

	double result = vector_dot(v1, v2);
	mu_assert("incorrect vector dot product", result == 20.0);

	return 0;
}

static char *
test_cross_product()
{
	struct vector v1 = { 1.0, 0.0, 0.0 };
	struct vector v2 = { 0.0, 1.0, 0.0 };

	struct vector result = vector_cross(v1, v2);
	struct vector expected = { 0.0, 0.0, 1.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_add()
{
	struct vector v1 = { 1.0, 2.0, 3.0 };
	struct vector v2 = { 2.0, 3.0, 4.0 };

	struct vector result = vector_add(v1, v2);
	struct vector expected = { 3.0, 5.0, 7.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_subtract()
{
	struct vector v1 = { 1.0, 5.0, 0.0 };
	struct vector v2 = { 2.0, 3.0, 0.0 };

	struct vector result = vector_subtract(v1, v2);
	struct vector expected = { -1.0, 2.0, 0.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_scalar_multiply()
{
	struct vector v1 = { 1.0, 2.0, 3.0 };
	double scalar = 5.0;

	struct vector result = vector_multiply(v1, scalar);
	struct vector expected = { 5.0, 10.0, 15.0 };
	mu_assert("vector not equal to expected",
	          vector_equals(result, expected));

	return 0;
}

static char *
test_length()
{
	struct vector v = { 1.0, 2.0, 3.0 };

	double expected = sqrt(1 + 4 + 9);
	double result = vector_len(v);
	mu_assert("length not equal to expected", result == expected);

	return 0;
}

static char *
test_normalise()
{
	struct vector v = { 3.0, 4.0, 5.0 };
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
	mu_run_test(test_cross_product);
	mu_run_test(test_add);
	mu_run_test(test_subtract);
	mu_run_test(test_scalar_multiply);
	mu_run_test(test_length);
	mu_run_test(test_normalise);
	return 0;
}
