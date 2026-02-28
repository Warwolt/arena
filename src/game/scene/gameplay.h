#pragma once

#include "engine/resource.h"

typedef struct Game Game;

typedef struct Gameplay {
	struct {
		TextureID background_texture_id;
	} resources;
} Gameplay;

void Gameplay_initialize(Game* game);
void Gameplay_update(Game* game);
void Gameplay_render(const Game* game);
