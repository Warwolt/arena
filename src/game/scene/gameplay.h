#pragma once

#include "engine/resource.h"

typedef struct Game Game;

typedef struct Gameplay {
	TextureID bg_texture_id;
	int room_width;
	int room_height;
} Gameplay;

void Gameplay_initialize(Game* game);
void Gameplay_update(Game* game);
void Gameplay_render(const Game* game);
