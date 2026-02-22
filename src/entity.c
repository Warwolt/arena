#include "entity.h"

EntityID EntityManager_add_entity(EntityManager* entities) {
	EntityID id = { entities->entities.size + 1 };
	Entity entity = { .id = id };
	Map_insert(&entities->entities, id.value, entity);
	return id;
}

void EntityManager_add_position(EntityManager* entities, EntityID id, Vector2 position) {
	if (Map_insert(&entities->components.positions, id.value, position)) {
		Entity* entity = NULL;
		Map_get_ptr(&entities->entities, id.value, &entity);
		entity->components[entity->num_components++] = ComponentType_Position;
	}
}

void EntityManager_get_position(EntityManager* entities, EntityID id, Vector2* position) {
	Map_get(&entities->components.positions, id.value, position);
}

void EntityManager_set_position(EntityManager* entities, EntityID id, Vector2 position) {
	Map_set(&entities->components.positions, id.value, position);
}

void EntityManager_add_sprite(EntityManager* entities, EntityID id, Sprite sprite) {
	if (Map_insert(&entities->components.sprites, id.value, sprite)) {
		Entity* entity = NULL;
		Map_get_ptr(&entities->entities, id.value, &entity);
		entity->components[entity->num_components++] = ComponentType_Sprite;
	}
}

void EntityManager_get_sprite(EntityManager* entities, EntityID id, Sprite* sprite) {
	Map_get(&entities->components.sprites, id.value, sprite);
}

void EntityManager_set_sprite(EntityManager* entities, EntityID id, Sprite sprite) {
	Map_set(&entities->components.sprites, id.value, sprite);
}
