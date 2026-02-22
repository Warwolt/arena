#pragma once

#include "component.h"
#include "map.h"
#include "resource.h"

#include <raylib.h>
#include <stddef.h>

#define MAX_NUM_ENTITES 128
#define MAX_POSITION_COMPONENTS 128
#define MAX_SPRITE_COMPONENTS 128

typedef struct EntityID {
	size_t value;
} EntityID;

typedef struct Entity {
	EntityID id;
} Entity;

typedef struct ComponentStore {
	Map(Vector2, MAX_POSITION_COMPONENTS) positions;
	Map(Sprite, MAX_SPRITE_COMPONENTS) sprites;
} ComponentStore;

typedef struct EntityManager {
	Entity entities[MAX_NUM_ENTITES];
	size_t num_entities;
	ComponentStore components;
} EntityManager;

EntityID EntityManager_add_entity(EntityManager* entities);
bool EntityManager_remove_entity(EntityID id);

void EntityManager_add_position(EntityManager* entities, EntityID id, Vector2 position);
void EntityManager_get_position(EntityManager* entities, EntityID id, Vector2* position);
void EntityManager_set_position(EntityManager* entities, EntityID id, Vector2 position);

void EntityManager_add_sprite(EntityManager* entities, EntityID id, Sprite sprite);
void EntityManager_get_sprite(EntityManager* entities, EntityID id, Sprite* sprite);
void EntityManager_set_sprite(EntityManager* entities, EntityID id, Sprite sprite);
