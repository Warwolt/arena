#include "entity.h"

EntityID EntityID_new(void) {
	static int next_id = 1;
	return (EntityID) { next_id++ };
}
