#pragma once

#include "engine/entity.h"
#include "engine/resource.h"
#include "game/scene.h"

#include <raylib.h>

#include <stdbool.h>

typedef struct Game {
	bool should_quit;
	bool show_debug_overlay;
	RenderTexture screen;
	ResourceManager resources;
	EntityManager entities;
	Scene scene;
} Game;

__declspec(dllexport) void Game_initialize(Game* game, int argc, char** argv);
__declspec(dllexport) void Game_shutdown(Game* game);
__declspec(dllexport) void Game_update(Game* game);
__declspec(dllexport) void Game_render(const Game* game);

void Game_quit(Game* game);
void Game_switch_scene(Game* game, SceneID scene_id);

Rectangle Game_screen_rect(const Game* game);
