#pragma once

#include <stddef.h>

typedef struct Arena Arena;

Arena* Arena_allocate(size_t size);
void* Arena_push(Arena* arena, size_t size);
void Arena_free(Arena* arena);
