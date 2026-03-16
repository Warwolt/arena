// Array with vector-like methods and bounds checking

#pragma once

#include "core/data/template_util.h"
#include "core/debug/assert.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Parameter checks */
#ifndef ARRAY_NAME
#error #define ARRAY_NAME to be the array type name before including this header!
#endif

#ifndef ARRAY_TYPE
#error #define ARRAY_TYPE to be the item type before including this header!
#endif

#ifndef ARRAY_CAPACITY
#error #define ARRAY_CAPACITY to be the max number of items before including this header!
#endif

/* Type definition */
typedef struct ARRAY_NAME {
	ARRAY_TYPE values[ARRAY_CAPACITY];
	size_t num_values;
} ARRAY_NAME;

/* Method definitions */
#define ARRAY_METHOD(name) JOIN(ARRAY_NAME, _##name)

static inline void ARRAY_METHOD(push)(ARRAY_NAME* array, ARRAY_TYPE item) {
	ASSERT(array->num_values < ARRAY_CAPACITY, "Trying to push to %s at max num values (capacity is %d)", TO_STRING(ARRAY_NAME), ARRAY_CAPACITY);
	array->values[array->num_values++] = item;
}

static inline void ARRAY_METHOD(pop)(ARRAY_NAME* array) {
	if (array->num_values > 0) {
		memset(&array->values[array->num_values - 1], 0, sizeof(ARRAY_TYPE));
		array->num_values--;
	}
}

/* Cleanup */
#undef ARRAY_NAME
#undef ARRAY_TYPE
#undef ARRAY_CAPACITY
#undef ARRAY_METHOD
