#include <rktest/rktest.h>

#include "memory/pool.h"

#include <stdbool.h>

#define TEST_POOL_CAPACITY (int)128
typedef struct TestPool {
	EntityID keys[TEST_POOL_CAPACITY];
	int values[TEST_POOL_CAPACITY];
	int size;
} TestPool;

// Pool_add_item
// Pool_remove_item
// Pool_get_item
// Pool_set_item

#define Pool_get_item(pool, item) Pool_get_item_impl((pool)->keys, item)

bool Pool_get_item_impl(EntityID* keys, void* item) {
	(void)(keys);
	(void)(item);
	return false;
}

TEST(PoolTests, GetItem_EmptyPool_DoesNothing) {
	TestPool pool = { 0 };

	int item = 0;
	bool did_get = Pool_get_item(&pool, &item);

	EXPECT_FALSE_INFO(did_get, "Empty pool unexpectedly returned item %d\n", item);
}
