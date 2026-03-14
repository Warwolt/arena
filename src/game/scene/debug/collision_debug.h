#pragma once

#include "core/geometry.h"

#include <stdbool.h>

typedef struct Game Game;

#define DEBUG_SHAPES_MAX 2

typedef struct CollisionDebugScene {
	float time_now;
	Shape shapes[DEBUG_SHAPES_MAX];
	int num_shapes;
	bool is_overlapping;
	int page;
} CollisionDebugScene;

void CollisionDebugScene_initialize(Game* game);
void CollisionDebugScene_update(Game* game);
void CollisionDebugScene_render(const Game* game);
