#include "core/data/array.h"

#include "core/debug/assert.h"

#include <string.h>

bool Array_push_impl(size_t elem_size, char* array_values, size_t* array_num_values, size_t array_capacity, char* value) {
	ASSERT(array_capacity > 0, "Trying to push to zero-initialized array. Missing call to Array_initialize?");
	if (*array_num_values >= array_capacity) {
		return false;
	}
	memcpy(array_values + elem_size * *array_num_values, value, elem_size); // array_values[array_num_values] = value
	*array_num_values += 1;
	return true;
}
