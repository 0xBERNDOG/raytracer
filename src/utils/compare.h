#ifndef COMPARE_H
#define COMPARE_H

#include <math.h>

#define FUZZY_EQUALS_TOLERANCE 0.00001
#define FUZZY_EQUALS(x, y) (fabs(x - y) <= 0.00001)

#endif