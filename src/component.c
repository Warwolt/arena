#include "component.h"

void ComponentManager_add_position(ComponentManager* components, EntityID id, Vector2 position) {
	Map_insert(&components->positions, id.value, position);
}

void ComponentManager_get_position(ComponentManager* components, EntityID id, Vector2* position) {
	Map_get(&components->positions, id.value, position);
}

void ComponentManager_set_position(ComponentManager* components, EntityID id, Vector2 position) {
	Map_set(&components->positions, id.value, position);
}

void ComponentManager_add_sprite(ComponentManager* components, EntityID id, Sprite sprite) {
	Map_insert(&components->sprites, id.value, sprite);
}

void ComponentManager_get_sprite(ComponentManager* components, EntityID id, Sprite* sprite) {
	Map_get(&components->sprites, id.value, sprite);
}

void ComponentManager_set_sprite(ComponentManager* components, EntityID id, Sprite sprite) {
	Map_set(&components->sprites, id.value, sprite);
}
