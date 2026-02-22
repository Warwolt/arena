#include "entity.h"

EntityID EntityManager_add_entity(EntityManager* entities) {
	entities->num_entities++;
	return (EntityID) { entities->num_entities };
}

void EntityManager_add_position(EntityManager* entities, EntityID id, Vector2 position) {
	Map_insert(&entities->components.positions, id.value, position);
}

void EntityManager_get_position(EntityManager* entities, EntityID id, Vector2* position) {
	Map_get(&entities->components.positions, id.value, position);
}

void EntityManager_set_position(EntityManager* entities, EntityID id, Vector2 position) {
	Map_set(&entities->components.positions, id.value, position);
}

void EntityManager_add_sprite(EntityManager* entities, EntityID id, Sprite sprite) {
	Map_insert(&entities->components.sprites, id.value, sprite);
}

void EntityManager_get_sprite(EntityManager* entities, EntityID id, Sprite* sprite) {
	Map_get(&entities->components.sprites, id.value, sprite);
}

void EntityManager_set_sprite(EntityManager* entities, EntityID id, Sprite sprite) {
	Map_set(&entities->components.sprites, id.value, sprite);
}
