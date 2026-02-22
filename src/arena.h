// Simple "bump allocator".
//
// Memory is allocated all at once from the operating system with `Arena_allocate`.
// Bytes from that memory can be grabbed with `Arena_push`, which just takes the next N.
// All bump allocations done with `Arena_push` are freed all at once with `Arena_free`.

#pragma once

#include <stddef.h>

typedef struct Arena Arena;

Arena* Arena_allocate(size_t size);
void* Arena_push(Arena* arena, size_t size);
void Arena_free(Arena* arena);
