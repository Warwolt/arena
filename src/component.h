#pragma once

#include "entity.h"
#include "map.h"
#include "resource.h"

#include <raylib.h>

typedef struct Sprite {
	TextureID texture_id;
	Rectangle clip_rect;
} Sprite;

#define MAX_POSITION_COMPONENTS (int)128
#define MAX_SPRITE_COMPONENTS (int)128

typedef struct ComponentManager {
	Map(Vector2, MAX_POSITION_COMPONENTS) positions;
	Map(Sprite, MAX_SPRITE_COMPONENTS) sprites;
} ComponentManager;

void ComponentManager_add_position(ComponentManager* components, EntityID id, Vector2 position);
void ComponentManager_get_position(ComponentManager* components, EntityID id, Vector2* position);
void ComponentManager_set_position(ComponentManager* components, EntityID id, Vector2 position);

void ComponentManager_add_sprite(ComponentManager* components, EntityID id, Sprite sprite);
void ComponentManager_get_sprite(ComponentManager* components, EntityID id, Sprite* sprite);
void ComponentManager_set_sprite(ComponentManager* components, EntityID id, Sprite sprite);
