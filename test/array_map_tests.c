#include <rktest/rktest.h>

// Write an ArrayMap implementation in the same style as the SparseArray
// We let the user store the data, and operate on it with functions that only deal with bytes, with
// type-aware macro wrappers.
// The ArrayMap is a Map<String, T> implementation with a fixed N capacity of items (i.e. an array).
//
// Interface
//
//   struct ArrayMap<T, N> {
//       const char keys[KEY_MAX][N];
//       T values[N];
//       size_t num_items;
//   }
//
// Methods
//
// - ArrayMap_insert
// - ArrayMap_remove
// - ArrayMap_get
// - ArrayMap_get_ptr
// - ArrayMap_set
// - ArrayMap_contains

#define ARRAY_MAP_KEY_LENGTH 128
#define ARRAY_MAP_MISSING_KEY -1

#define MAX_TEST_ITEMS 64
typedef struct TestArrayMap {
	char keys[ARRAY_MAP_KEY_LENGTH][MAX_TEST_ITEMS];
	int values[MAX_TEST_ITEMS];
	size_t num_values;
} TestArrayMap;

int TestArrayMap_key_index(TestArrayMap* map, const char* key) {
	for (size_t i = 0; i < map->num_values; i++) {
		if (strcmp(map->keys[i], key) == 0) {
			return (int)i;
		}
	}
	return ARRAY_MAP_MISSING_KEY;
}

bool TestArrayMap_insert(TestArrayMap* map, const char* key, int value) {
	/* Skip empty key */
	if (key[0] == '\0') {
		return false;
	}

	/* If key exists, update value*/
	const int maybe_index = TestArrayMap_key_index(map, key);
	if (maybe_index != ARRAY_MAP_MISSING_KEY) {
		map->values[maybe_index] = value;
		return true;
	}

	/* Push new value */
	const size_t index = map->num_values++;
	strncpy_s(map->keys[index], ARRAY_MAP_KEY_LENGTH, key, _TRUNCATE);
	map->values[index] = value;
	return true;
}

bool TestArrayMap_get(TestArrayMap* map, const char* key, int* value) {
	const int maybe_index = TestArrayMap_key_index(map, key);
	if (maybe_index == ARRAY_MAP_MISSING_KEY) {
		return false;
	}
	*value = map->values[maybe_index];
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
