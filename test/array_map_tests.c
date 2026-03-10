#include <rktest/rktest.h>

#include "core/array_map.h"

#define ARRAY_MAP_MISSING_KEY -1

typedef ArrayMap(int, 64) TestArrayMap;

#define ArrayMap_insert(map, key, value)             \
	ArrayMap_insert_impl(                            \
		sizeof((map)->values[0]),                    \
		(char (*)[ARRAY_MAP_KEY_LENGTH])(map)->keys, \
		(char*)(map)->values,                        \
		&(map)->num_values,                          \
		key,                                         \
		(char*)&value                                \
	)

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

int TestArrayMap_key_index(TestArrayMap* map, const char* key) {
	for (size_t i = 0; i < map->num_values; i++) {
		if (strcmp(map->keys[i], key) == 0) {
			return (int)i;
		}
	}
	return ARRAY_MAP_MISSING_KEY;
}

bool TestArrayMap_insert(TestArrayMap* map, const char* key, int value) {
	return ArrayMap_insert(map, key, value);
}

void TestArrayMap_remove(TestArrayMap* map, const char* key) {
	const int maybe_index = TestArrayMap_key_index(map, key);
	if (maybe_index != ARRAY_MAP_MISSING_KEY) {
		// Replace item to remove with last
		map->values[maybe_index] = map->values[map->num_values - 1];
		strncpy_s(map->keys[maybe_index], ARRAY_MAP_KEY_LENGTH, map->keys[map->num_values - 1], _TRUNCATE);
		map->num_values--;
	}
}

bool TestArrayMap_get(TestArrayMap* map, const char* key, int* value) {
	const int maybe_index = TestArrayMap_key_index(map, key);
	if (maybe_index == ARRAY_MAP_MISSING_KEY) {
		return false;
	}
	*value = map->values[maybe_index];
	return true;
}

bool TestArrayMap_get_ptr(TestArrayMap* map, const char* key, int** value_ptr) {
	const int maybe_index = TestArrayMap_key_index(map, key);
	if (maybe_index == ARRAY_MAP_MISSING_KEY) {
		return false;
	}
	*value_ptr = &map->values[maybe_index];
	return true;
}

bool TestArrayMap_contains(TestArrayMap* map, const char* key) {
	return TestArrayMap_key_index(map, key) != ARRAY_MAP_MISSING_KEY;
}

TEST(ArrayMapTests, InsertElement_GetElement) {
	TestArrayMap map = { 0 };
	char* key = "foo";

	bool did_insert = TestArrayMap_insert(&map, key, 1234);
	int value = 0;
	bool did_get = TestArrayMap_get(&map, key, &value);
	bool contains_element = TestArrayMap_contains(&map, key);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_TRUE(contains_element);
	EXPECT_EQ(value, 1234);
	EXPECT_EQ((int)map.num_values, 1);
}

TEST(ArrayMapTests, InsertMultipleElements_GetThoseElements) {
	TestArrayMap map = { 0 };
	const char* key1 = "aa";
	const char* key2 = "bb";
	const char* key3 = "cc";

	TestArrayMap_insert(&map, key1, 11);
	TestArrayMap_insert(&map, key2, 22);
	TestArrayMap_insert(&map, key3, 33);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestArrayMap_get(&map, key1, &value1);
	bool did_get2 = TestArrayMap_get(&map, key2, &value2);
	bool did_get3 = TestArrayMap_get(&map, key3, &value3);
	bool contains_element1 = TestArrayMap_contains(&map, key1);
	bool contains_element2 = TestArrayMap_contains(&map, key2);
	bool contains_element3 = TestArrayMap_contains(&map, key3);

	EXPECT_TRUE(did_get1);
	EXPECT_TRUE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_TRUE(contains_element1);
	EXPECT_TRUE(contains_element2);
	EXPECT_TRUE(contains_element3);
	EXPECT_EQ(value1, 11);
	EXPECT_EQ(value2, 22);
	EXPECT_EQ(value3, 33);
	EXPECT_EQ((int)map.num_values, 3);
}

TEST(ArrayMapTests, InsertElement_EmptyKey_DoesNothing) {
	TestArrayMap map = { 0 };
	const char* empty_key = "";

	bool did_insert = TestArrayMap_insert(&map, empty_key, 1234);
	int value = 0;
	bool did_get = TestArrayMap_get(&map, empty_key, &value);
	bool contains_zero = TestArrayMap_contains(&map, empty_key);

	EXPECT_FALSE(did_insert);
	EXPECT_FALSE(did_get);
	EXPECT_FALSE(contains_zero);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)map.num_values, 0);
}

TEST(ArrayMapTests, InsertElement_ExistingKey_UpdatesValue) {
	TestArrayMap array = { 0 };
	const char* key = "bar";

	TestArrayMap_insert(&array, key, 12);
	bool did_insert = TestArrayMap_insert(&array, key, 34);
	int value = 0;
	bool did_get = TestArrayMap_get(&array, key, &value);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 34);
	EXPECT_EQ((int)array.num_values, 1);
}

TEST(ArrayMapTests, InsertElements_RemoveElement) {
	TestArrayMap array = { 0 };
	const char* key = "Alice";

	TestArrayMap_insert(&array, key, 1001);
	TestArrayMap_remove(&array, key);

	int value = 0;
	bool did_get = TestArrayMap_get(&array, key, &value);
	bool contains_element = TestArrayMap_contains(&array, key);

	EXPECT_FALSE(did_get);
	EXPECT_FALSE(contains_element);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)array.num_values, 0);
}

TEST(ArrayMapTests, InsertMultipleElements_RemoveOneElement_GetRemaining) {
	TestArrayMap array = { 0 };
	const char* key1 = "1";
	const char* key2 = "2";
	const char* key3 = "3";

	TestArrayMap_insert(&array, key1, 10);
	TestArrayMap_insert(&array, key2, 20);
	TestArrayMap_insert(&array, key3, 30);
	TestArrayMap_remove(&array, key2);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestArrayMap_get(&array, key1, &value1);
	bool did_get2 = TestArrayMap_get(&array, key2, &value2);
	bool did_get3 = TestArrayMap_get(&array, key3, &value3);
	bool contains_element1 = TestArrayMap_contains(&array, key1);
	bool contains_element2 = TestArrayMap_contains(&array, key2);
	bool contains_element3 = TestArrayMap_contains(&array, key3);

	EXPECT_TRUE(did_get1);
	EXPECT_FALSE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_TRUE(contains_element1);
	EXPECT_FALSE(contains_element2);
	EXPECT_TRUE(contains_element3);
	EXPECT_EQ(value1, 10);
	EXPECT_EQ(value2, 0);
	EXPECT_EQ(value3, 30);
	EXPECT_EQ((int)array.num_values, 2);
}

TEST(ArrayMapTests, InsertElement_GetElementPointer) {
	TestArrayMap array = { 0 };
	const char* key = "Bob";

	TestArrayMap_insert(&array, key, 1234);
	int* value_ptr = 0;
	TestArrayMap_get_ptr(&array, key, &value_ptr);
	*value_ptr = 4567;
	int value = 0;
	TestArrayMap_get(&array, key, &value);

	EXPECT_EQ(value, 4567);
}
