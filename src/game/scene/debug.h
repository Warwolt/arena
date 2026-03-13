#pragma once

#include <stdbool.h>

typedef struct Game Game;

typedef struct DebugScene {
	int current_page;
	bool just_pushed_page; // this seems really clumsy, can't se store this inside UIContext?
} DebugScene;

void DebugScene_initialize(Game* game);
void DebugScene_update(Game* game);
void DebugScene_render(const Game* game);
