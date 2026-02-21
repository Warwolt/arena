#include <rktest/rktest.h>

#include <stddef.h>

#define MAX_TEST_ITEMS 64
typedef struct TestDict {
	size_t indices[MAX_TEST_ITEMS]; // key -> index
	int keys[MAX_TEST_ITEMS]; // index -> key
	int values[MAX_TEST_ITEMS]; // index -> value
	size_t size;
} TestDict;

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
	memcpy(dict_values + index * elem_size, value, elem_size);
	*dict_size += 1;
	return true;
}

bool TestDict_insert(TestDict* dict, int key, int value) {
	return Dict_insert_impl(sizeof(dict->values[0]), dict->indices, dict->keys, (char*)dict->values, &dict->size, key, (char*)&value);
}

void TestDict_remove(TestDict* dict, int key) {
	/* Skip the zero-key */
	if (key == 0) {
		return;
	}

	/* Check if key exists */
	const size_t index = dict->indices[key - 1];
	const bool key_exists = dict->keys[index] == key;
	if (!key_exists) {
		return;
	}

	/* Replace element to remove with last element */
	const size_t last_index = dict->size - 1;
	const int last_key = dict->keys[last_index];
	dict->indices[key - 1] = 0;
	dict->indices[last_key - 1] = index;
	dict->keys[index] = dict->keys[last_index];
	dict->values[index] = dict->values[last_index];

	/* Remove last element */
	dict->keys[last_index] = 0;
	dict->values[last_index] = 0;
	dict->size -= 1;
}

bool TestDict_get(TestDict* dict, int key, int* value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Check if key exists */
	const size_t index = dict->indices[key - 1];
	const bool key_exists = dict->keys[index] == key;
	if (!key_exists) {
		return false;
	}

	/* Return value */
	*value = dict->values[index];
	return true;
}

bool TestDict_set(TestDict* dict, int key, int value) {
	/* Skip the zero-key */
	if (key == 0) {
		return false;
	}

	/* Update value if key exists */
	const size_t index = dict->indices[key - 1];
	const bool key_exists = dict->keys[index] == key;
	if (!key_exists) {
		return false;
	}

	dict->values[index] = value;
	return true;
}

TEST(DictTests, InsertElement_GetElement) {
	TestDict dict = { 0 };
	int key = 11;

	bool did_insert = TestDict_insert(&dict, key, 1234);
	int value = 0;
	bool did_get = TestDict_get(&dict, key, &value);

	EXPECT_TRUE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 1234);
	EXPECT_EQ((int)dict.size, 1);
}

TEST(DictTests, InsertMultipleElements_GetThoseElements) {
	TestDict dict = { 0 };
	int key1 = 1;
	int key2 = 2;
	int key3 = 3;

	TestDict_insert(&dict, key1, 11);
	TestDict_insert(&dict, key2, 22);
	TestDict_insert(&dict, key3, 33);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestDict_get(&dict, key1, &value1);
	bool did_get2 = TestDict_get(&dict, key2, &value2);
	bool did_get3 = TestDict_get(&dict, key3, &value3);

	EXPECT_TRUE(did_get1);
	EXPECT_TRUE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_EQ(value1, 11);
	EXPECT_EQ(value2, 22);
	EXPECT_EQ(value3, 33);
	EXPECT_EQ((int)dict.size, 3);
}

TEST(DictTests, InsertElement_ZeroKey_DoesNothing) {
	TestDict dict = { 0 };
	int zero_key = 0;

	bool did_insert = TestDict_insert(&dict, zero_key, 1234);
	int value = 0;
	bool did_get = TestDict_get(&dict, zero_key, &value);

	EXPECT_FALSE(did_insert);
	EXPECT_FALSE(did_get);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)dict.size, 0);
}

TEST(DictTests, InsertElement_ExistingKey_DoesNothing) {
	TestDict dict = { 0 };
	int key = 11;

	TestDict_insert(&dict, key, 12);
	bool did_insert = TestDict_insert(&dict, key, 34);
	int value = 0;
	bool did_get = TestDict_get(&dict, key, &value);

	EXPECT_FALSE(did_insert);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 12);
	EXPECT_EQ((int)dict.size, 1);
}

TEST(DictTests, SetElement_MissingKey_DoesNotUpdate) {
	TestDict dict = { 0 };
	int key = 1;

	bool did_set = TestDict_set(&dict, key, 1234);
	int value = 0;
	bool did_get = TestDict_get(&dict, key, &value);

	EXPECT_FALSE(did_set);
	EXPECT_FALSE(did_get);
	EXPECT_EQ(value, 0);
}

TEST(DictTests, SetElement_ExistingKey_UpdatesValue) {
	TestDict dict = { 0 };
	int key = 1;

	TestDict_insert(&dict, key, 1234);
	bool did_set = TestDict_set(&dict, key, 5678);
	int value = 0;
	bool did_get = TestDict_get(&dict, key, &value);

	EXPECT_TRUE(did_set);
	EXPECT_TRUE(did_get);
	EXPECT_EQ(value, 5678);
}

TEST(DictTests, InsertElements_RemoveElement) {
	TestDict dict = { 0 };
	int key = 1;

	TestDict_insert(&dict, key, 1001);
	TestDict_remove(&dict, key);

	int value = 0;
	bool did_get = TestDict_get(&dict, key, &value);

	EXPECT_FALSE(did_get);
	EXPECT_EQ(value, 0);
	EXPECT_EQ((int)dict.size, 0);
}

TEST(DictTests, InsertMultipleElements_RemoveOneElement_GetRemaining) {
	TestDict dict = { 0 };
	int key1 = 1;
	int key2 = 2;
	int key3 = 3;

	TestDict_insert(&dict, key1, 10);
	TestDict_insert(&dict, key2, 20);
	TestDict_insert(&dict, key3, 30);
	TestDict_remove(&dict, key2);

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	bool did_get1 = TestDict_get(&dict, key1, &value1);
	bool did_get2 = TestDict_get(&dict, key2, &value2);
	bool did_get3 = TestDict_get(&dict, key3, &value3);

	EXPECT_TRUE(did_get1);
	EXPECT_FALSE(did_get2);
	EXPECT_TRUE(did_get3);
	EXPECT_EQ(value1, 10);
	EXPECT_EQ(value2, 0);
	EXPECT_EQ(value3, 30);
	EXPECT_EQ((int)dict.size, 2);
}
