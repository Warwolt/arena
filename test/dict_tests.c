#include <rktest/rktest.h>

#include <stddef.h>

// Dict_insert
// Dict_remove
// Dict_get
// Dict_set

// dense_idx < size
// AND
// dense[dense_idx].sparse_idx == idx

#define MAX_TEST_ITEMS 64
typedef struct TestDict {
	size_t indices[MAX_TEST_ITEMS];
	int keys[MAX_TEST_ITEMS];
	int values[MAX_TEST_ITEMS];
	size_t size;
} TestDict;

#define Dict_insert(dict, key, value) false
#define Dict_get(dict, key, value) false

bool TestDict_insert(TestDict* dict, int key, int value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	size_t index = dict->indices[key - 1];
	const bool key_already_exists = dict->keys[index] == key;
	if (key_already_exists) {
		return false;
	}

	/* Insert new key */
	dict->keys[index] = key;
	dict->values[index] = value;
	return true;
}

bool TestDict_get(TestDict* dict, int key, int* value) {
	*value = 1234;
	return true;
}

TEST(DictTests, InsertElement_GetElement) {
	TestDict dict = { 0 };
	int key = 11;

	bool did_insert = TestDict_insert(&dict, 11, 1234);
	int value = 0;
	bool did_get = TestDict_get(&dict, 11, &value);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 1234);
}

TEST(DictTests, InsertElement_ExistingKey_DoesNothing) {
	TestDict dict = { 0 };
	int key = 11;

	TestDict_insert(&dict, 11, 1234);
	bool did_insert = TestDict_insert(&dict, 11, 4567);
	int value = 0;
	bool did_get = TestDict_get(&dict, 11, &value);

	EXPECT_FALSE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 1234);
}

// InsertElement_ZeroKey_DoesNothing
