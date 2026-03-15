#pragma once

#include "core/geometry.h"

#include <stdbool.h>

typedef struct Game Game;

#define DEBUG_SHAPES_MAX 3

// FIXME: can we skip the int-based indexing and use EntityID instead? Then we
// actually have a reasonable NULL value, and we're closer to what we'd do in
// real engine code.
typedef struct CollisionDebugScene {
	float time_now;
	int num_shapes;
	Shape shapes[DEBUG_SHAPES_MAX];
	bool shape_is_overlapping[DEBUG_SHAPES_MAX];
	int mouse_grabed_shape; // 1-indexed
	int page;
} CollisionDebugScene;

void CollisionDebugScene_initialize(Game* game);
void CollisionDebugScene_update(Game* game);
void CollisionDebugScene_render(const Game* game);
