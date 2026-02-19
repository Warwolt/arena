#include "arena.h"

#include <stdint.h>
#include <stdlib.h>

struct Arena {
	void* data;
	size_t size;
};

Arena* Arena_allocate(size_t size) {
	Arena* arena = (Arena*)malloc(sizeof(Arena));
	*arena = (Arena) {
		.data = malloc(size),
		.size = size,
	};
	return arena;
}

void* Arena_push(Arena* arena, size_t size) {
	// FIXME: we should check the capacity and only push if not already full
	void* ptr = (uint8_t*)arena->data + arena->size;
	arena->size += size;
	return ptr;
}

void Arena_free(Arena* arena) {
	free(arena->data);
	free(arena);
}
