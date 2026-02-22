#include "memory/dict.h"

#include <string.h>

bool Dict_insert_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, size_t* dict_size, int key, char* value) {
    /* Skip the zero-key */
    if (key == 0) {
        return false;
    }

    /* Check if key already exists */
    const size_t maybe_index = dict_indices[key - 1];
    const bool key_exists = dict_keys[maybe_index] == key;
    if (key_exists) {
        return false;
    }

    /* Insert new key */
    const size_t index = *dict_size;
    dict_indices[key - 1] = index;
    dict_keys[index] = key;
    memcpy(dict_values + index * elem_size, value, elem_size); // dict->values[index] = value;
    *dict_size += 1;
    return true;
}

void Dict_remove_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, size_t* dict_size, int key) {
    /* Skip the zero-key */
    if (key == 0) {
        return;
    }

    /* Check if key exists */
    const size_t index = dict_indices[key - 1];
    const bool key_exists = dict_keys[index] == key;
    if (!key_exists) {
        return;
    }

    /* Replace element to remove with last element */
    const size_t last_index = *dict_size - 1;
    const int last_key = dict_keys[last_index];
    dict_indices[key - 1] = 0;
    dict_indices[last_key - 1] = index;
    dict_keys[index] = dict_keys[last_index];
    memcpy(dict_values + index * elem_size, dict_values + last_index * elem_size, elem_size); // dict->values[index] = dict_values[last_index];

    /* Remove last element */
    dict_keys[last_index] = 0;
    memset(dict_values + last_index * elem_size, 0, elem_size); // dict->values[last_index] = 0;
    *dict_size -= 1;
}

bool Dict_get_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, int key, char* value_out) {
    /* Skip the zero-key */
    if (key == 0) {
        return false;
    }

    /* Check if key exists */
    const size_t index = dict_indices[key - 1];
    const bool key_exists = dict_keys[index] == key;
    if (!key_exists) {
        return false;
    }

    /* Return value */
    memcpy(value_out, dict_values + index * elem_size, elem_size); // *value_out = dict_values[index];
    return true;
}

bool Dict_set_impl(size_t elem_size, size_t* dict_indices, int* dict_keys, char* dict_values, int key, char* value) {
    /* Skip the zero-key */
    if (key == 0) {
        return false;
    }

    /* Update value if key exists */
    const size_t index = dict_indices[key - 1];
    const bool key_exists = dict_keys[index] == key;
    if (!key_exists) {
        return false;
    }

    memcpy(dict_values + index * elem_size, value, elem_size); // dict->values[index] = value;
    return true;
}
