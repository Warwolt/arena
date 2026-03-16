#include <rktest/rktest.h>

#include "core/data/array.h"

typedef Array(int, 64) TestArray;

// TODO:
// Can we maybe try a different approach to the containers?
// There's a macro style of generics that's used here:
// - https://github.com/stclib/STC/blob/main/docs/vec_api.md
//
// We'd have:
//      #define NAME TestArray
// 		#define TYPE int
//      #define SIZE 64
// 		#include <core/data/array.h>
//
// The implementation array.h can then use the values in `T` when implementing
// the methods. We don't have to do the void* based stuff, which means we get to
// pass actual values to the methods without writing wrapper functions.
//
// I think this strikes an ok balance between obviously needing _some_ generic
// containers, but not going overboard with it? It's still C after all.
//
// (We _have_ to be able to map Int -> T and String -> T to write meaningful
// programs, so we _need_ something like the SparseArray and ArrayMap types,
// which in turn could benefit from Array for bounds checking)

TestArray TestArray_initialize(void) {
	TestArray array;
	Array_initialize(&array);
	return array;
}

bool TestArray_push(TestArray* array, int value) {
	return Array_push(array, &value);
}

TEST(ArrayTests, ZeroInitializedArray_PushItem_Asserts) {
	TestArray array = { 0 };
	EXPECT_DEATH(TestArray_push(&array, 123), "*Trying to push to zero-initialized array. Missing call to Array_initialize?");
}

TEST(ArrayTests, InitializedArray_PushItem_ItemGetsPushed) {
	TestArray array = TestArray_initialize();

	bool did_push = TestArray_push(&array, 123);

	EXPECT_TRUE(did_push);
	EXPECT_EQ((int)array.capacity, 64);
	ASSERT_EQ((int)array.num_values, 1);
	EXPECT_EQ(array.values[0], 123);
}

TEST(ArrayTests, FullArray_PushItem_ItemNotPushed) {
	TestArray array = TestArray_initialize();

	array.num_values = array.capacity;
	bool did_push = TestArray_push(&array, 123);

	EXPECT_FALSE(did_push);
	EXPECT_EQ((int)array.capacity, 64);
	ASSERT_EQ((int)array.num_values, 64);
}
