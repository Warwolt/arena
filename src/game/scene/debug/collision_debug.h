#pragma once

#include "core/geometry.h"

#include <stdbool.h>

typedef struct Game Game;

#define DEBUG_SHAPES_MAX 3

typedef struct CollisionDebugScene {
	float time_now;
	int num_shapes;
	Shape shapes[DEBUG_SHAPES_MAX];
	bool shape_is_overlapping[DEBUG_SHAPES_MAX];
	bool manual_control;
	int page;
} CollisionDebugScene;

void CollisionDebugScene_initialize(Game* game);
void CollisionDebugScene_update(Game* game);
void CollisionDebugScene_render(const Game* game);
