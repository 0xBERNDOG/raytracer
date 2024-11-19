#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <stdbool.h>

#define optional_type(TYPE)                                                    \
	struct {                                                               \
		bool present;                                                  \
		TYPE value;                                                    \
	}

#define CREATE_OPTIONAL(TYPE, VALUE)                                           \
	(optional_##TYPE)                                                      \
	{                                                                      \
		.present = true, .value = VALUE                                \
	}

typedef optional_type(double) optional_double;

#endif
