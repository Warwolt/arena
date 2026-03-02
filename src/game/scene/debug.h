#pragma once

typedef struct Game Game;

typedef struct DebugScene {
	int dummy;
} DebugScene;

void DebugScene_initialize(Game* game);
void DebugScene_update(Game* game);
void DebugScene_render(const Game* game);
