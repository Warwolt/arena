#pragma once

#include "engine/entity.h"
#include "engine/resource.h"
#include "game/scene.h"
#include "platform/window.h"

#include <raylib.h>

#include <stdbool.h>

typedef struct Game {
	bool should_quit;
	bool show_debug_overlay;
	Font system_font;
	Window window;
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
void Game_draw_text(const Game* game, const char* text, int x, int y, int font_size, Color color);
int Game_measure_text_width(const Game* game, const char* text, int font_size);
