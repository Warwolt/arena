#pragma once

typedef struct EntityID {
	int value;
} EntityID;

EntityID EntityID_new(void);
int EntityID_count(void);
