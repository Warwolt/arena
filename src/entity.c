#include "entity.h"

static size_t g_num_entities;

EntityID EntityID_new(void) {
	g_num_entities++;
	int id = (int)g_num_entities;
	return (EntityID) { id };
}

size_t EntityID_count(void) {
	return g_num_entities;
}
