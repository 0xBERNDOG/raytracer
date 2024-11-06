#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <stdbool.h>

#define optional_type(type)                                                    \
	struct {                                                               \
		bool present;                                                  \
		type value;                                                    \
	}

#endif
