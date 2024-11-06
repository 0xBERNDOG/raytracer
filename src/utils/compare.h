#ifndef COMPARE_H
#define COMPARE_H

#include <math.h>

#define FUZZY_EQUALS_TOLERANCE 1e-10
#define FUZZY_EQUALS(x, y)     (fabs(x - y) < FUZZY_EQUALS_TOLERANCE)

#endif