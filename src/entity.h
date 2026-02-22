#pragma once

#include <stddef.h>

typedef struct EntityID {
	int value;
} EntityID;

EntityID EntityID_new(void);
size_t EntityID_count(void);
