#include "memory/arena.h"

#include <stdint.h>
#include <stdlib.h>

struct Arena {
	void* data; // memory allocated to the arena
	size_t size; // amount of bytes currently in use
	size_t capacity; // total amount of allocated bytes
};

Arena* Arena_allocate(size_t size) {
	Arena* arena = (Arena*)malloc(sizeof(Arena));
	*arena = (Arena) {
		.data = malloc(size),
		.size = 0,
		.capacity = size,
	};
	return arena;
}

void* Arena_push(Arena* arena, size_t size) {
	if (arena->size + size > arena->capacity) {
		return NULL;
	}
	void* ptr = (uint8_t*)arena->data + arena->size;
	arena->size += size;
	return ptr;
}

void Arena_free(Arena* arena) {
	free(arena->data);
	free(arena);
}
