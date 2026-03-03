#pragma once

typedef struct Game Game;

typedef struct DebugScene {
	int current_page;
	int focused_menu_item;
} DebugScene;

void DebugScene_initialize(Game* game);
void DebugScene_update(Game* game);
void DebugScene_render(const Game* game);
