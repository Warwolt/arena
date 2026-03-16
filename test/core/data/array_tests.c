#include <rktest/rktest.h>

#define ARRAY_NAME TestArray
#define ARRAY_TYPE int
#define ARRAY_CAPACITY 64
#include "core/data/array.h"

TEST(ArrayTests, PushItem_ItemGetsPushed) {
	TestArray array = { 0 };

	TestArray_push(&array, 123);

	ASSERT_EQ((int)array.num_values, 1);
	EXPECT_EQ(array.values[0], 123);
}

TEST(ArrayTests, PushItem_PopItem) {
	TestArray array = { 0 };

	TestArray_push(&array, 10);
	TestArray_push(&array, 20);
	TestArray_pop(&array);

	ASSERT_EQ((int)array.num_values, 1);
	EXPECT_EQ(array.values[0], 10);
	EXPECT_EQ_INFO(array.values[1], 0, "Expected popped item to have been zeroed out\n");
}

TEST(ArrayTests, PushItem_ArrayIsFull_Asserts) {
	TestArray array = { 0 };

	array.num_values = 64;
	EXPECT_DEATH(TestArray_push(&array, 123), "*Trying to push to TestArray at max num values (capacity is 64)");
}
