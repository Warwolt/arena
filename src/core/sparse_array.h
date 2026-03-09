// Sparse array implementation (integer keyed map)
//
// Interface:
//
//  struct SparseArray<T, N> {
//  	size_t indices[N]; // key -> index
//  	size_t keys[N]; // index -> key
//  	T values[N]; // index -> value
//  	size_t size;
//  };
//
// Supports keys in range 1 to N.
// Zero is reserved as a null-value.
//
// `indices` sparse array maps key-1 to a dense-array-index.
// `keys` dense array maps dense-array-index back into a key.
// `value` dense array contains values.

#pragma once

#include <stdbool.h>
#include <stddef.h>

// SparseArray struct
#define SparseArray(Type, Capacity) \
	struct {                        \
		size_t indices[Capacity];   \
		size_t keys[Capacity];      \
		Type values[Capacity];      \
		size_t size;                \
	}

// Insert a new element into the array
#define SparseArray_insert(array, key, value) \
	SparseArray_insert_impl(                  \
		sizeof((array)->values[0]),           \
		(array)->indices,                     \
		(array)->keys,                        \
		(char*)(array)->values,               \
		&(array)->size,                       \
		(key),                                \
		(char*)&(value)                       \
	)

// Remove an element by its key
#define SparseArray_remove(array, key) \
	SparseArray_remove_impl(           \
		sizeof((array)->values[0]),    \
		(array)->indices,              \
		(array)->keys,                 \
		(char*)(array)->values,        \
		&(array)->size,                \
		(key)                          \
	)

// Get the value corresponding to key if it exists
#define SparseArray_get(array, key, value_out) \
	SparseArray_get_impl(sizeof((array)->values[0]), (array)->indices, (array)->keys, (char*)(array)->values, (key), (char*)(value_out))

// Get a pointer to value corresponding to key if it exists
#define SparseArray_get_ptr(array, key, value_ptr_out) \
	SparseArray_get_ptr_impl(sizeof((array)->values[0]), (array)->indices, (array)->keys, (char*)(array)->values, (key), (char**)(value_ptr_out))

// Set the value corresponding to a key
#define SparseArray_set(array, key, value) \
	SparseArray_set_impl(sizeof((array)->values[0]), (array)->indices, (array)->keys, (char*)(array)->values, (key), (char*)&(value))

// Check if array contains key
#define SparseArray_contains(array, key) SparseArray_contains_impl((array)->indices, (array)->keys, (key))

bool SparseArray_insert_impl(size_t elem_size, size_t* array_indices, size_t* array_keys, char* array_values, size_t* array_size, size_t key, char* value);
bool SparseArray_remove_impl(size_t elem_size, size_t* array_indices, size_t* array_keys, char* array_values, size_t* array_size, size_t key);
bool SparseArray_get_impl(size_t elem_size, const size_t* array_indices, const size_t* array_keys, const char* array_values, size_t key, char* value_out);
bool SparseArray_get_ptr_impl(size_t elem_size, const size_t* array_indices, const size_t* array_keys, const char* array_values, size_t key, const char** value_ptr_out);
bool SparseArray_set_impl(size_t elem_size, size_t* array_indices, size_t* array_keys, char* array_values, size_t key, char* value);
bool SparseArray_contains_impl(size_t* array_indices, size_t* array_keys, size_t key);
