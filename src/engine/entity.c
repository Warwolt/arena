#include "engine/entity.h"

EntityID EntityManager_add_entity(EntityManager* entities, Vector2 position) {
	EntityID id = { 0 };

	/* Either re-use discarded ID or add a new one */
	if (entities->num_discarded_ids > 0) {
		id = entities->discarded_ids[entities->num_discarded_ids - 1];
		entities->num_discarded_ids--;
	} else {
		id = (EntityID) { entities->entities.num_values + 1 };
	}

	Entity entity = {
		.id = id,
		.position = position,
	};
	SparseArray_insert(&entities->entities, id.value, entity);
	return id;
}

bool EntityManager_remove_entity(EntityManager* entities, EntityID id) {
	Entity* entity = NULL;
	SparseArray_get_ptr(&entities->entities, id.value, &entity);
	if (entity) {
		/* Remove components */
		for (size_t i = 0; i < entity->num_components; i++) {
			switch (entity->components[i]) {
				case ComponentType_None:
					break;
				case ComponentType_Sprite:
					SparseArray_remove(&entities->components.sprites, id.value);
					break;
				case ComponentType_CollisionShape:
					SparseArray_remove(&entities->components.collision_shapes, id.value);
					break;
			}
		}

		/* Remove entity */
		SparseArray_remove(&entities->entities, id.value);
		entities->discarded_ids[entities->num_discarded_ids++] = id;
	}
	return entity != NULL;
}

bool EntityManager_has_component(const EntityManager* entities, EntityID id, ComponentType component) {
	const Entity* entity = NULL;
	SparseArray_get_ptr(&entities->entities, id.value, &entity);
	if (!entity) {
		return false;
	}
	for (size_t i = 0; i < entity->num_components; i++) {
		if (entity->components[i] == component) {
			return true;
		}
	}
	return false;
}

bool EntityManager_get_position(const EntityManager* entities, EntityID id, Vector2* position) {
	Entity* entity = NULL;
	SparseArray_get_ptr(&entities->entities, id.value, &entity);
	if (entity) {
		*position = entity->position;
	}
	return entity != NULL;
}

void EntityManager_set_position(EntityManager* entities, EntityID id, Vector2 position) {
	Entity* entity = NULL;
	SparseArray_get_ptr(&entities->entities, id.value, &entity);
	if (entity) {
		entity->position = position;
	}
}

void EntityManager_add_sprite(EntityManager* entities, EntityID id, Sprite sprite) {
	if (SparseArray_insert(&entities->components.sprites, id.value, sprite)) {
		Entity* entity = NULL;
		SparseArray_get_ptr(&entities->entities, id.value, &entity);
		entity->components[entity->num_components++] = ComponentType_Sprite;
	}
}

bool EntityManager_get_sprite(const EntityManager* entities, EntityID id, Sprite* sprite) {
	return SparseArray_get(&entities->components.sprites, id.value, sprite);
}

void EntityManager_set_sprite(EntityManager* entities, EntityID id, Sprite sprite) {
	SparseArray_insert(&entities->components.sprites, id.value, sprite);
}

void EntityManager_add_collision_shape(EntityManager* entities, EntityID id, Shape shape) {
	if (SparseArray_insert(&entities->components.collision_shapes, id.value, shape)) {
		Entity* entity = NULL;
		SparseArray_get_ptr(&entities->entities, id.value, &entity);
		entity->components[entity->num_components++] = ComponentType_CollisionShape;
	}
}

bool EntityManager_get_collision_shape(const EntityManager* entities, EntityID id, Shape* shape) {
	return SparseArray_get(&entities->components.collision_shapes, id.value, shape);
}

void EntityManager_set_collision_shape(EntityManager* entities, EntityID id, Shape shape) {
	SparseArray_insert(&entities->components.collision_shapes, id.value, shape);
}
