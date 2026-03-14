#pragma once

#include "core/geometry.h"

#include <stdbool.h>

typedef struct Game Game;

typedef struct CollisionDebugScene {
	float time_now;
	Circle circles[2];
	bool is_overlapping;
	int page;
} CollisionDebugScene;

void CollisionDebugScene_initialize(Game* game);
void CollisionDebugScene_update(Game* game);
void CollisionDebugScene_render(const Game* game);
