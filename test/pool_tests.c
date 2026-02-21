#include <rktest/rktest.h>

#include "memory/pool.h"

#include <stdbool.h>

#define MAX_TEST_ITEMS (int)128
typedef struct TestPool {
	EntityID keys[MAX_TEST_ITEMS];
	int values[MAX_TEST_ITEMS];
	int size;
} TestPool;

TEST(PoolTests, EmptyPool_GetItem_DoesNothing) {
	TestPool pool = { 0 };
	EntityID id = { 1 };

	int item = -1;
	Pool_get_item(&pool, id, &item);

	EXPECT_EQ(item, -1);
}

TEST(PoolTests, AddItem_GetItem_ItemRetrieved) {
	TestPool pool = { 0 };
	EntityID id = { 1 };
	int item = 123;

	Pool_add_item(&pool, id, item);
	int retrieved_item = 0;
	Pool_get_item(&pool, id, &retrieved_item);

	EXPECT_EQ(item, retrieved_item);
}

TEST(PoolTests, AddItem_RemoveItem_CannotGetItem) {
	TestPool pool = { 0 };
	EntityID id = { 1 };

	const int item = 123;
	Pool_add_item(&pool, id, item);

	Pool_remove_item(&pool, id);

	int retrieved_item = 0;
	Pool_get_item(&pool, id, &retrieved_item);

	EXPECT_EQ(item, retrieved_item);
}

TEST(PoolTests, SetItem_GetItem_RetrievesUpdatedItem) {
	TestPool pool = { 0 };
	EntityID id = { 1 };

	Pool_add_item(&pool, id, 123);
	Pool_set_item(&pool, id, 456);
	int item = 0;
	Pool_get_item(&pool, id, &item);

	EXPECT_EQ(item, 456);
}
