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

// #define ARRAY_MAP_KEY_LENGTH 128

// #define MAX_TEST_ITEMS 64
// typedef struct TestArrayMap {
// 	const char keys[ARRAY_MAP_KEY_LENGTH][MAX_TEST_ITEMS];
// 	int values[MAX_TEST_ITEMS];
// 	size_t num_items;
// } TestArrayMap;

// TEST(ArrayMapTests, InsertElement_GetElement) {
// 	TestArrayMap array = { 0 };
// 	char* key = "foo";

// 	bool did_insert = TestArrayMap_insert(&array, key, 1234);
// 	int value = 0;
// 	bool did_get = TestArrayMap_get(&array, key, &value);
// 	bool contains_element = TestArrayMap_contains(&array, key);

// 	EXPECT_TRUE(did_insert);
// 	EXPECT_TRUE(did_get);
// 	EXPECT_TRUE(contains_element);
// 	EXPECT_EQ(value, 1234);
// 	EXPECT_EQ((int)array.size, 1);
// }
