#pragma once

// String-keyed map based on a fixed-size array.
//
// Interface:
//
//  struct ArrayMap<T, N> {
//	    char keys[N][ARRAY_MAP_KEY_LENGTH];
//	    T values[N];
//	    size_t num_values;
//  };
//

#include <stdbool.h>
#include <stddef.h>

#define ARRAY_MAP_KEY_LENGTH 128

// ArrayMap struct
#define ArrayMap(Type, Capacity)                   \
	struct {                                       \
		char keys[Capacity][ARRAY_MAP_KEY_LENGTH]; \
		Type values[Capacity];                     \
		size_t num_values;                         \
	}
