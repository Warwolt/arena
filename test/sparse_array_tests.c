#include <rktest/rktest.h>

#include "core/sparse_array.h"

#define MAX_TEST_ITEMS 64
typedef struct TestSparseArray {
	size_t indices[MAX_TEST_ITEMS]; // key -> index
	size_t keys[MAX_TEST_ITEMS]; // index -> key
	int values[MAX_TEST_ITEMS]; // index -> value
	size_t num_values;
} TestSparseArray;

bool TestSparseArray_insert(TestSparseArray* array, size_t key, int value) {
	return SparseArray_insert(array, key, value);
}

void TestSparseArray_remove(TestSparseArray* array, size_t key) {
	SparseArray_remove(array, key);
}

bool TestSparseArray_get(TestSparseArray* array, size_t key, int* value) {
	return SparseArray_get(array, key, value);
}

bool TestSparseArray_get_ptr(TestSparseArray* array, size_t key, int** value) {
	return SparseArray_get_ptr(array, key, value);
}

bool TestSparseArray_contains(TestSparseArray* array, size_t key) {
	return SparseArray_contains(array, key);
}

int* TestSparseArray_begin(TestSparseArray* array) {
	return &array->values[0];
}

int* TestSparseArray_end(TestSparseArray* array) {
	return &array->values[array->num_values];
}

TEST(SparseArrayTests, InsertElement_GetElement) {
	TestSparseArray array = { 0 };
	size_t key = 11;

	bool did_insert = TestSparseArray_insert(&array, key, 1234);
	int value = 0;
	bool did_get = TestSparseArray_get(&array, key, &value);
	bool contains_element = TestSparseArray_contains(&array, key);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_TRUE(contains_element);
	EXPECT_EQ(value, 1234);
	EXPECT_EQ((int)array.num_values, 1);
}

TEST(SparseArrayTests, InsertMultipleElements_GetThoseElements) {
	TestSparseArray array = { 0 };
	size_t key1 = 1;
	size_t key2 = 2;
	size_t key3 = 3;

	TestSparseArray_insert(&array, key1, 11);
	TestSparseArray_insert(&array, key2, 22);
	TestSparseArray_insert(&array, key3, 33);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestSparseArray_get(&array, key1, &value1);
	bool did_get2 = TestSparseArray_get(&array, key2, &value2);
	bool did_get3 = TestSparseArray_get(&array, key3, &value3);
	bool contains_element1 = TestSparseArray_contains(&array, key1);
	bool contains_element2 = TestSparseArray_contains(&array, key2);
	bool contains_element3 = TestSparseArray_contains(&array, key3);

	EXPECT_TRUE(did_get1);
	EXPECT_TRUE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_TRUE(contains_element1);
	EXPECT_TRUE(contains_element2);
	EXPECT_TRUE(contains_element3);
	EXPECT_EQ(value1, 11);
	EXPECT_EQ(value2, 22);
	EXPECT_EQ(value3, 33);
	EXPECT_EQ((int)array.num_values, 3);
}

TEST(SparseArrayTests, InsertElement_ZeroKey_DoesNothing) {
	TestSparseArray array = { 0 };
	int zero_key = 0;

	bool did_insert = TestSparseArray_insert(&array, zero_key, 1234);
	int value = 0;
	bool did_get = TestSparseArray_get(&array, zero_key, &value);
	bool contains_zero = TestSparseArray_contains(&array, zero_key);

	EXPECT_FALSE(did_insert);
	EXPECT_FALSE(did_get);
	EXPECT_FALSE(contains_zero);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)array.num_values, 0);
}

TEST(SparseArrayTests, InsertElement_ExistingKey_UpdatesValue) {
	TestSparseArray array = { 0 };
	size_t key = 11;

	TestSparseArray_insert(&array, key, 12);
	bool did_insert = TestSparseArray_insert(&array, key, 34);
	int value = 0;
	bool did_get = TestSparseArray_get(&array, key, &value);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 34);
	EXPECT_EQ((int)array.num_values, 1);
}

TEST(SparseArrayTests, InsertElements_RemoveElement) {
	TestSparseArray array = { 0 };
	size_t key = 1;

	TestSparseArray_insert(&array, key, 1001);
	TestSparseArray_remove(&array, key);

	int value = 0;
	bool did_get = TestSparseArray_get(&array, key, &value);
	bool contains_element = TestSparseArray_contains(&array, key);

	EXPECT_FALSE(did_get);
	EXPECT_FALSE(contains_element);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)array.num_values, 0);
}

TEST(SparseArrayTests, InsertMultipleElements_RemoveOneElement_GetRemaining) {
	TestSparseArray array = { 0 };
	size_t key1 = 1;
	size_t key2 = 2;
	size_t key3 = 3;

	TestSparseArray_insert(&array, key1, 10);
	TestSparseArray_insert(&array, key2, 20);
	TestSparseArray_insert(&array, key3, 30);
	TestSparseArray_remove(&array, key2);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestSparseArray_get(&array, key1, &value1);
	bool did_get2 = TestSparseArray_get(&array, key2, &value2);
	bool did_get3 = TestSparseArray_get(&array, key3, &value3);
	bool contains_element1 = TestSparseArray_contains(&array, key1);
	bool contains_element2 = TestSparseArray_contains(&array, key2);
	bool contains_element3 = TestSparseArray_contains(&array, key3);

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

TEST(SparseArrayTests, InsertElement_GetElementPointer) {
	TestSparseArray array = { 0 };
	size_t key = 11;

	TestSparseArray_insert(&array, key, 1234);
	int* value_ptr = 0;
	TestSparseArray_get_ptr(&array, key, &value_ptr);
	*value_ptr = 4567;
	int value = 0;
	TestSparseArray_get(&array, key, &value);

	EXPECT_EQ(value, 4567);
}
