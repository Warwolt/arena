#include "rktest.h"

#include "arena.h"

TEST(ArenaTests, PushedAllocationsAreConsecutive) {
	Arena* arena = Arena_allocate(100);

	int* one = Arena_push(arena, sizeof(int));
	int* two = Arena_push(arena, sizeof(int));
	int bytes_between_pointers = (int)((char*)two - (char*)one);

	EXPECT_EQ(bytes_between_pointers, sizeof(int));
	Arena_free(arena);
}

TEST(ArenaTests, PushBeyondCapacity_ReturnsNull) {
	Arena* arena = Arena_allocate(1);

	int* ptr = Arena_push(arena, sizeof(int));

	EXPECT_PTR_EQ(ptr, NULL);
}
