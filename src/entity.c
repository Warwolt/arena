#include "entity.h"

EntityID EntityManager_add_entity(EntityManager* entities) {
	EntityID id = { 0 };

	/* Either re-use discarded ID or add a new one */
	if (entities->num_discarded_ids > 0) {
		id = entities->discarded_ids[entities->num_discarded_ids - 1];
		entities->num_discarded_ids--;
	} else {
		id = (EntityID) { entities->entities.size + 1 };
	}

	Entity entity = { .id = id };
	Map_insert(&entities->entities, id.value, entity);
	return id;
}

bool EntityManager_remove_entity(EntityManager* entities, EntityID id) {
	Entity* entity = NULL;
	Map_get_ptr(&entities->entities, id.value, &entity);
	if (entity) {
		/* Remove components */
		for (size_t i = 0; i < entity->num_components; i++) {
			switch (entity->components[i]) {
				case ComponentType_Position:
					Map_remove(&entities->components.positions, id.value);
					break;
				case ComponentType_Sprite:
					Map_remove(&entities->components.sprites, id.value);
					break;
			}
		}

		/* Remove entity */
		Map_remove(&entities->entities, id.value);
		entities->discarded_ids[entities->num_discarded_ids++] = id;
	}
	return entity != NULL;
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
