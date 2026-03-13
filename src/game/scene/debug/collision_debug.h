#pragma once

#include <stdbool.h>

typedef struct Game Game;

typedef struct CollisionDebugScene {
	int x;
} CollisionDebugScene;

void CollisionDebugScene_initialize(Game* game);
void CollisionDebugScene_update(Game* game);
void CollisionDebugScene_render(const Game* game);
