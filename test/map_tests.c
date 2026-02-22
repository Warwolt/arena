#include <rktest/rktest.h>

#include "memory/map.h"

#define MAX_TEST_ITEMS 64
typedef struct TestMap {
	size_t indices[MAX_TEST_ITEMS]; // key -> index
	int keys[MAX_TEST_ITEMS]; // index -> key
	int values[MAX_TEST_ITEMS]; // index -> value
	size_t size;
} TestMap;

bool TestMap_insert(TestMap* map, int key, int value) {
	return Map_insert(map, key, value);
}

void TestMap_remove(TestMap* map, int key) {
	Map_remove(map, key);
}

bool TestMap_get(TestMap* map, int key, int* value) {
	return Map_get(map, key, value);
}

bool TestMap_set(TestMap* map, int key, int value) {
	return Map_set(map, key, value);
}

bool TestMap_contains(TestMap* map, int key) {
	return Map_contains(map, key);
}

TEST(MapTests, InsertElement_GetElement) {
	TestMap map = { 0 };
	int key = 11;

	bool did_insert = TestMap_insert(&map, key, 1234);
	int value = 0;
	bool did_get = TestMap_get(&map, key, &value);
	bool contains_element = TestMap_contains(&map, key);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_TRUE(contains_element);
	EXPECT_EQ(value, 1234);
	EXPECT_EQ((int)map.size, 1);
}

TEST(MapTests, InsertMultipleElements_GetThoseElements) {
	TestMap map = { 0 };
	int key1 = 1;
	int key2 = 2;
	int key3 = 3;

	TestMap_insert(&map, key1, 11);
	TestMap_insert(&map, key2, 22);
	TestMap_insert(&map, key3, 33);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestMap_get(&map, key1, &value1);
	bool did_get2 = TestMap_get(&map, key2, &value2);
	bool did_get3 = TestMap_get(&map, key3, &value3);
	bool contains_element1 = TestMap_contains(&map, key1);
	bool contains_element2 = TestMap_contains(&map, key2);
	bool contains_element3 = TestMap_contains(&map, key3);

	EXPECT_TRUE(did_get1);
	EXPECT_TRUE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_TRUE(contains_element1);
	EXPECT_TRUE(contains_element2);
	EXPECT_TRUE(contains_element3);
	EXPECT_EQ(value1, 11);
	EXPECT_EQ(value2, 22);
	EXPECT_EQ(value3, 33);
	EXPECT_EQ((int)map.size, 3);
}

TEST(MapTests, InsertElement_ZeroKey_DoesNothing) {
	TestMap map = { 0 };
	int zero_key = 0;

	bool did_insert = TestMap_insert(&map, zero_key, 1234);
	int value = 0;
	bool did_get = TestMap_get(&map, zero_key, &value);
	bool contains_zero = TestMap_contains(&map, zero_key);

	EXPECT_FALSE(did_insert);
	EXPECT_FALSE(did_get);
	EXPECT_FALSE(contains_zero);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)map.size, 0);
}

TEST(MapTests, InsertElement_ExistingKey_DoesNothing) {
	TestMap map = { 0 };
	int key = 11;

	TestMap_insert(&map, key, 12);
	bool did_insert = TestMap_insert(&map, key, 34);
	int value = 0;
	bool did_get = TestMap_get(&map, key, &value);

	EXPECT_FALSE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 12);
	EXPECT_EQ((int)map.size, 1);
}

TEST(MapTests, SetElement_MissingKey_DoesNotUpdate) {
	TestMap map = { 0 };
	int key = 1;

	bool did_set = TestMap_set(&map, key, 1234);
	int value = 0;
	bool did_get = TestMap_get(&map, key, &value);

	EXPECT_FALSE(did_set);
	EXPECT_FALSE(did_get);
	EXPECT_EQ(value, 0);
}

TEST(MapTests, SetElement_ExistingKey_UpdatesValue) {
	TestMap map = { 0 };
	int key = 1;

	TestMap_insert(&map, key, 1234);
	bool did_set = TestMap_set(&map, key, 5678);
	int value = 0;
	bool did_get = TestMap_get(&map, key, &value);

	EXPECT_TRUE(did_set);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 5678);
}

TEST(MapTests, InsertElements_RemoveElement) {
	TestMap map = { 0 };
	int key = 1;

	TestMap_insert(&map, key, 1001);
	TestMap_remove(&map, key);

	int value = 0;
	bool did_get = TestMap_get(&map, key, &value);
	bool contains_element = TestMap_contains(&map, key);

	EXPECT_FALSE(did_get);
	EXPECT_FALSE(contains_element);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)map.size, 0);
}

TEST(MapTests, InsertMultipleElements_RemoveOneElement_GetRemaining) {
	TestMap map = { 0 };
	int key1 = 1;
	int key2 = 2;
	int key3 = 3;

	TestMap_insert(&map, key1, 10);
	TestMap_insert(&map, key2, 20);
	TestMap_insert(&map, key3, 30);
	TestMap_remove(&map, key2);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestMap_get(&map, key1, &value1);
	bool did_get2 = TestMap_get(&map, key2, &value2);
	bool did_get3 = TestMap_get(&map, key3, &value3);
	bool contains_element1 = TestMap_contains(&map, key1);
	bool contains_element2 = TestMap_contains(&map, key2);
	bool contains_element3 = TestMap_contains(&map, key3);

	EXPECT_TRUE(did_get1);
	EXPECT_FALSE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_TRUE(contains_element1);
	EXPECT_FALSE(contains_element2);
	EXPECT_TRUE(contains_element3);
	EXPECT_EQ(value1, 10);
	EXPECT_EQ(value2, 0);
	EXPECT_EQ(value3, 30);
	EXPECT_EQ((int)map.size, 2);
}
