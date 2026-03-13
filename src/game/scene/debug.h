#pragma once

#include <stdbool.h>

typedef struct Game Game;

#define MenuStack_MaxDepth 4
typedef struct MenuStack {
	int menus[MenuStack_MaxDepth];
	int num_menus;
	int prev_num_menus;
	bool has_pushed_menu;
} MenuStack;

typedef struct DebugScene {
	MenuStack menu_stack;
} DebugScene;

void DebugScene_initialize(Game* game);
void DebugScene_update(Game* game);
void DebugScene_render(const Game* game);
