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

#define MAX_TEST_ITEMS 64
typedef struct TestArrayMap {
	char keys[ARRAY_MAP_KEY_LENGTH][MAX_TEST_ITEMS];
	int values[MAX_TEST_ITEMS];
	size_t num_values;
} TestArrayMap;

bool TestArrayMap_insert(TestArrayMap* map, const char* key, int value) {
	// FIXME: skip the empty key

	strncpy_s(map->keys[0], ARRAY_MAP_KEY_LENGTH, key, _TRUNCATE);
	map->values[0] = value;
	map->num_values++;
	return true;
}

bool TestArrayMap_get(TestArrayMap* map, const char* key, int* value) {
	*value = map->values[0];
	return true;
}

bool TestArrayMap_contains(TestArrayMap* map, const char* key) {
	return true;
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
