#include "entity.h"

static int g_num_entities;

EntityID EntityID_new(void) {
	return (EntityID) { ++g_num_entities };
}

int EntityID_count(void) {
	return g_num_entities;
}
