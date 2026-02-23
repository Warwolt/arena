#pragma once

#include "component.h"
#include "map.h"
#include "resource.h"

#include <raylib.h>
#include <stddef.h>

#define MAX_NUM_ENTITES 128
#define MAX_NUM_ENTIY_COMPONENTS 32
#define MAX_POSITION_COMPONENTS 128
#define MAX_SPRITE_COMPONENTS 128
#define MAX_COLLISION_SHAPE_COMPONENTS 128

typedef struct EntityID {
	size_t value;
} EntityID;

typedef struct Entity {
	EntityID id;
	Vector2 position;
	ComponentType components[MAX_NUM_ENTIY_COMPONENTS];
	size_t num_components;
} Entity;

typedef struct ComponentStore {
	Map(Vector2, MAX_POSITION_COMPONENTS) positions;
	Map(Sprite, MAX_SPRITE_COMPONENTS) sprites;
	Map(Shape, MAX_SPRITE_COMPONENTS) collision_shapes;
} ComponentStore;

typedef struct EntityManager {
	Map(Entity, MAX_NUM_ENTITES) entities;
	ComponentStore components;
	EntityID discarded_ids[MAX_NUM_ENTITES];
	size_t num_discarded_ids;
} EntityManager;

EntityID EntityManager_add_entity(EntityManager* entities, Vector2 position);
bool EntityManager_remove_entity(EntityManager* entities, EntityID id);

void EntityManager_add_position(EntityManager* entities, EntityID id, Vector2 position);
bool EntityManager_get_position(EntityManager* entities, EntityID id, Vector2* position);
void EntityManager_set_position(EntityManager* entities, EntityID id, Vector2 position);

void EntityManager_add_sprite(EntityManager* entities, EntityID id, Sprite sprite);
bool EntityManager_get_sprite(EntityManager* entities, EntityID id, Sprite* sprite);
void EntityManager_set_sprite(EntityManager* entities, EntityID id, Sprite sprite);

void EntityManager_add_collision_shape(EntityManager* entities, EntityID id, Shape shape);
bool EntityManager_get_collision_shape(EntityManager* entities, EntityID id, Shape* shape);
void EntityManager_set_collision_shape(EntityManager* entities, EntityID id, Shape shape);
