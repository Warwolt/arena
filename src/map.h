// Integer-keyed map implementation
//
// Implemented as a sparse array.
// Supports keys in range 1-N.
// Zero is reserved as a "null" value.
//
// Interface:
//
//  struct Map<T,N> {
//  	size_t indices[N]; // key -> index
//  	size_t keys[N]; // index -> key
//  	T values[N]; // index -> value
//  	size_t size;
//  };
//
// `indices` sparse array maps key-1 to a dense-array-index.
// `keys` dense array maps dense-array-index back into a key.
// `value` dense array contains values.

#pragma once

#include <stdbool.h>
#include <stddef.h>

// Map struct
#define Map(Type, Capacity)       \
	struct {                      \
		size_t indices[Capacity]; \
		size_t keys[Capacity];    \
		Type values[Capacity];    \
		size_t size;              \
	}

// Insert a new element into the map
#define Map_insert(map, key, value) \
	Map_insert_impl(sizeof((map)->values[0]), (map)->indices, (map)->keys, (char*)(map)->values, &(map)->size, (key), (char*)&(value))

// Remove an element by its key
#define Map_remove(map, key)                                                                                          \
	Map_remove_impl(sizeof((map)->values[0]), (map)->indices, (map)->keys, (char*)(map)->values, &(map)->size, (key))

// Get the value corresponding to key if it exists
#define Map_get(map, key, value_out) \
	Map_get_impl(sizeof((map)->values[0]), (map)->indices, (map)->keys, (char*)(map)->values, (key), (char*)(value_out))

// Get a pointer to value if corresponding to key if it exists
#define Map_get_ptr(map, key, value_ptr_out) \
	Map_get_ptr_impl(sizeof((map)->values[0]), (map)->indices, (map)->keys, (char*)(map)->values, (key), (char**)(value_ptr_out))

// Set the value corresponding to a key
#define Map_set(map, key, value)                                                                                      \
	Map_set_impl(sizeof((map)->values[0]), (map)->indices, (map)->keys, (char*)(map)->values, (key), (char*)&(value))

// Check if map contains key
#define Map_contains(map, key) Map_contains_impl((map)->indices, (map)->keys, (key))

bool Map_insert_impl(size_t elem_size, size_t* map_indices, size_t* map_keys, char* map_values, size_t* map_size, size_t key, char* value);
bool Map_remove_impl(size_t elem_size, size_t* map_indices, size_t* map_keys, char* map_values, size_t* map_size, size_t key);
bool Map_get_impl(size_t elem_size, size_t* map_indices, size_t* map_keys, char* map_values, size_t key, char* value_out);
bool Map_get_ptr_impl(size_t elem_size, size_t* map_indices, size_t* map_keys, char* map_values, size_t key, char** value_ptr_out);
bool Map_set_impl(size_t elem_size, size_t* map_indices, size_t* map_keys, char* map_values, size_t key, char* value);
bool Map_contains_impl(size_t* map_indices, size_t* map_keys, size_t key);
