// Assocative array implementation
//
// Positive number valued keys (zero is not a key).
// Zero is reserved as a "null" value.
//
// Interface:
//
//  struct Dict<T,N> {
//  	size_t indices[N]; // key -> index
//  	int keys[N]; // index -> key
//  	T values[N]; // index -> value
//  	size_t size;
//  };

#pragma once

#include <stdbool.h>
#include <stddef.h>

// Insert a new element into the dict
#define Dict_insert(dict, key, value) \
	Dict_insert_impl(sizeof((dict)->values[0]), (dict)->indices, (dict)->keys, (char*)(dict)->values, &(dict)->size, (key), (char*)&(value))

// Remove an element by its key
#define Dict_remove(dict, key) \
	Dict_remove_impl(sizeof((dict)->values[0]), (dict)->indices, (dict)->keys, (char*)(dict)->values, &(dict)->size, (key))

// Get the value corresponding to key if it exists
#define Dict_get(dict, key, value_out) \
	Dict_get_impl(sizeof((dict)->values[0]), (dict)->indices, (dict)->keys, (char*)(dict)->values, (key), (char*)(value_out))

// Set the value corresponding to a key
#define Dict_set(dict, key, value) \
	Dict_set_impl(sizeof((dict)->values[0]), (dict)->indices, (dict)->keys, (char*)(dict)->values, (key), (char*)&(value))

bool Dict_insert_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, size_t* dict_size, int key, char* value);
void Dict_remove_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, size_t* dict_size, int key);
bool Dict_get_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, int key, char* value_out);
bool Dict_set_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, int key, char* value);
