#pragma once

#include "engine/entity.h"
#include "engine/resource.h"

typedef struct Game Game;

typedef struct Gameplay {
	int room_width;
	int room_height;
	Camera2D camera;
	TextureID bg_texture_id;
	EntityID player_id;
} Gameplay;

void Gameplay_initialize(Game* game);
void Gameplay_update(Game* game);
void Gameplay_render(const Game* game);
