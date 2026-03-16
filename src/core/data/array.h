// Array with vector-like methods and bounds checking
//
// Interface:
//
//  struct Array<T, N> {
//  	T values[N];
//  	size_t num_values;
//      size_t capacity;
//  };

#pragma once

#include <stdbool.h>
#include <stddef.h>

// Array struct
#define Array(Type, Capacity)  \
	struct {                   \
		Type values[Capacity]; \
		size_t num_values;     \
		size_t capacity;       \
	}

#define Array_initialize(array)                                                   \
	do {                                                                          \
		(array)->num_values = 0;                                                  \
		(array)->capacity = sizeof((array)->values) / sizeof((array)->values[0]); \
	} while (0)

#define Array_push(array, value) \
	Array_push_impl(sizeof((array)->values[0]), (char*)(array)->values, &(array)->num_values, (array)->capacity, (char*)value)
#define Array_pop(array)

bool Array_push_impl(size_t elem_size, char* array_values, size_t* array_num_values, size_t array_capacity, char* value);
