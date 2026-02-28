#pragma once

#include "engine/resource.h"
#include "game/scene/scene.h"

#include <raylib.h>

#include <stdbool.h>

typedef struct Game {
	bool should_quit;
	RenderTexture screen;
	ResourceManager resources;
	Scene scene;
} Game;

void Game_initialize(Game* game, int screen_width, int screen_height);
void Game_update(Game* game);
void Game_render(const Game* game);

void Game_quit(Game* game);
void Game_switch_scene(Game* game, SceneID id);

Rectangle Game_screen_rect(const Game* game);
