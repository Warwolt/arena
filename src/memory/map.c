#include "memory/map.h"

#include <string.h>

bool Map_insert_impl(size_t elem_size, size_t* map_indices, int* map_keys, char* map_values, size_t* map_size, int key, char* value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key already exists */
	const size_t maybe_index = map_indices[key - 1];
	const bool key_exists = map_keys[maybe_index] == key;
	if (key_exists) {
		return false;
	}

	/* Insert new key */
	const size_t index = *map_size;
	map_indices[key - 1] = index;
	map_keys[index] = key;
	memcpy(map_values + index * elem_size, value, elem_size); // map->values[index] = value;
	*map_size += 1;
	return true;
}

bool Map_remove_impl(size_t elem_size, size_t* map_indices, int* map_keys, char* map_values, size_t* map_size, int key) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	const size_t index = map_indices[key - 1];
	const bool key_exists = map_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	/* Replace element to remove with last element */
	const size_t last_index = *map_size - 1;
	const int last_key = map_keys[last_index];
	map_indices[key - 1] = 0;
	map_indices[last_key - 1] = index;
	map_keys[index] = map_keys[last_index];
	memcpy(map_values + index * elem_size, map_values + last_index * elem_size, elem_size); // map->values[index] = map_values[last_index];

	/* Remove last element */
	map_keys[last_index] = 0;
	memset(map_values + last_index * elem_size, 0, elem_size); // map->values[last_index] = 0;
	*map_size -= 1;
	return true;
}

bool Map_get_impl(size_t elem_size, size_t* map_indices, int* map_keys, char* map_values, int key, char* value_out) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	const size_t index = map_indices[key - 1];
	const bool key_exists = map_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	/* Return value */
	memcpy(value_out, map_values + index * elem_size, elem_size); // *value_out = map_values[index];
	return true;
}

bool Map_set_impl(size_t elem_size, size_t* map_indices, int* map_keys, char* map_values, int key, char* value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Update value if key exists */
	const size_t index = map_indices[key - 1];
	const bool key_exists = map_keys[index] == key;
	if (!key_exists) {
		return false;
	}

	memcpy(map_values + index * elem_size, value, elem_size); // map->values[index] = value;
	return true;
}

bool Map_contains_impl(size_t* map_indices, int* map_keys, int key) {
	/* Never contains the zero-key */
	if (key == 0) {
		return false;
	}

	/* Contains element if key->index mapping and index->key mapping agree */
	return map_keys[map_indices[key - 1]] == key;
}
