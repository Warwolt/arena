#include "core/sparse_array.h"

#include <string.h>

bool SparseArray_insert_impl(size_t elem_size, size_t* array_indices, size_t* array_keys, char* array_values, size_t* array_size, size_t key, char* value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Just update value if key already exists */
	const size_t maybe_index = array_indices[key - 1];
	const bool key_exists = array_keys[maybe_index] == key;
	if (key_exists) {
		memcpy(array_values + maybe_index * elem_size, value, elem_size); // array->values[index] = value;
		return true;
	}

	/* Insert new key */
	const size_t index = *array_size;
	array_indices[key - 1] = index;
	array_keys[index] = key;
	memcpy(array_values + index * elem_size, value, elem_size); // array->values[index] = value;
	*array_size += 1;
	return true;
}

bool SparseArray_remove_impl(size_t elem_size, size_t* array_indices, size_t* array_keys, char* array_values, size_t* array_size, size_t key) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	const size_t index = array_indices[key - 1];
	const bool key_exists = array_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	/* Replace element to remove with last element */
	const size_t last_index = *array_size - 1;
	const size_t last_key = array_keys[last_index];
	array_indices[key - 1] = 0;
	array_indices[last_key - 1] = index;
	array_keys[index] = array_keys[last_index];
	memcpy(array_values + index * elem_size, array_values + last_index * elem_size, elem_size); // array->values[index] = array_values[last_index];

	/* Remove last element */
	array_keys[last_index] = 0;
	memset(array_values + last_index * elem_size, 0, elem_size); // array->values[last_index] = 0;
	*array_size -= 1;
	return true;
}

bool SparseArray_get_impl(size_t elem_size, const size_t* array_indices, const size_t* array_keys, const char* array_values, size_t key, char* value_out) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	const size_t index = array_indices[key - 1];
	const bool key_exists = array_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	/* Return value */
	memcpy(value_out, array_values + index * elem_size, elem_size); // *value_out = array_values[index];
	return true;
}

bool SparseArray_get_ptr_impl(size_t elem_size, const size_t* array_indices, const size_t* array_keys, const char* array_values, size_t key, const char** value_ptr_out) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	const size_t index = array_indices[key - 1];
	const bool key_exists = array_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	/* Return value */
	*value_ptr_out = array_values + index * elem_size;
	return true;
}

bool SparseArray_set_impl(size_t elem_size, size_t* array_indices, size_t* array_keys, char* array_values, size_t key, char* value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Update value if key exists */
	const size_t index = array_indices[key - 1];
	const bool key_exists = array_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	memcpy(array_values + index * elem_size, value, elem_size); // array->values[index] = value;
	return true;
}

bool SparseArray_contains_impl(size_t* array_indices, size_t* array_keys, size_t key) {
	/* Never contains the zero-key */
	if (key == 0) {
		return false;
	}

	/* Contains element if key->index mapping and index->key mapping agree */
	return array_keys[array_indices[key - 1]] == key;
}
