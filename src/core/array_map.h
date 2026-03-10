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

#define ArrayMap_insert(map, key, value)             \
	ArrayMap_insert_impl(                            \
		sizeof((map)->values[0]),                    \
		(char (*)[ARRAY_MAP_KEY_LENGTH])(map)->keys, \
		(char*)(map)->values,                        \
		&(map)->num_values,                          \
		key,                                         \
		(char*)&value                                \
	)

bool ArrayMap_insert_impl(size_t elem_size, char (*map_keys)[ARRAY_MAP_KEY_LENGTH], char* map_values, size_t* map_num_values, const char* key, char* value);