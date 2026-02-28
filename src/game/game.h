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

void Game_update(Game* game);
void Game_render(const Game* game);

inline void Game_quit(Game* game) {
	game->should_quit = true;
}

inline void Game_switch_scene(Game* game, SceneID id) {
	game->scene = (Scene) { .id = id };
}
