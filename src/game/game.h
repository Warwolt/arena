#pragma once

#include "engine/resource.h"
#include "game/scene.h"

#include <stdbool.h>

typedef struct Screen {
	int width;
	int height;
} Screen;

typedef struct Game {
	bool should_quit;
	Scene scene;
	ResourceManager resources;
	Screen screen;
} Game;

inline void Game_quit(Game* game) {
	game->should_quit = true;
}

inline void Game_switch_scene(Game* game, SceneID id) {
	game->scene = (Scene) { .id = id };
}
