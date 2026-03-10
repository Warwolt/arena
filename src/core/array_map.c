#include "core/array_map.h"

#include <string.h>

#define ARRAY_MAP_MISSING_KEY -1

int ArrayMap_key_index(char (*map_keys)[ARRAY_MAP_KEY_LENGTH], size_t map_num_values, const char* key) {
	for (size_t i = 0; i < map_num_values; i++) {
		if (strcmp(map_keys[i], key) == 0) {
			return (int)i;
		}
	}
	return ARRAY_MAP_MISSING_KEY;
}

bool ArrayMap_insert_impl(size_t elem_size, char (*map_keys)[ARRAY_MAP_KEY_LENGTH], char* map_values, size_t* map_num_values, const char* key, char* value) {
	/* Skip empty key */
	if (key[0] == '\0') {
		return false;
	}

	/* Update value if key already exists */
	const int maybe_index = ArrayMap_key_index(map_keys, *map_num_values, key);
	if (maybe_index != ARRAY_MAP_MISSING_KEY) {
		memcpy(map_values + maybe_index * elem_size, value, elem_size); // map->values[maybe_index] = value;
		return true;
	}

	/* Else, push new key-value pair */
	const size_t index = (*map_num_values)++;
	strncpy_s(map_keys[index], ARRAY_MAP_KEY_LENGTH, key, _TRUNCATE);
	memcpy(map_values + index * elem_size, value, elem_size); // map->values[index] = value;
	return true;
}

void ArrayMap_remove_impl(size_t elem_size, char (*map_keys)[ARRAY_MAP_KEY_LENGTH], char* map_values, size_t* map_num_values, const char* key) {
	const int maybe_index = ArrayMap_key_index(map_keys, *map_num_values, key);
	if (maybe_index != ARRAY_MAP_MISSING_KEY) {
		// Replace item to remove with last item
		memcpy(map_values + maybe_index * elem_size, map_values + (*map_num_values - 1) * elem_size, elem_size); // map->values[maybe_index] = map->values[map->num_values - 1];
		strncpy_s(map_keys[maybe_index], ARRAY_MAP_KEY_LENGTH, map_keys[*map_num_values - 1], _TRUNCATE);
		(*map_num_values)--;
	}
}
