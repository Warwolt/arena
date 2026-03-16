#pragma once

#include "core/geometry.h"

#define ARRAY_NAME ShapeArray
#define ARRAY_TYPE Shape
#define ARRAY_CAPACITY 64
#include "core/data/array.h"

#include <stdbool.h>

typedef struct Game Game;

#define DEBUG_SHAPES_MAX 3

typedef struct CollisionDebugScene {
	float time_now;
	int page;
	int mouse_grabed_shape;

	// FIXME:
	// Is this really any better than the previous style container?
	// "ShapeArray" doesn't include what length the array has, which reads awkward.
	//
	// It'd be great if we could write something like ShapeArray(64) and still
	// magically have all the methods work, but that way we lose access to the
	// ARRAY_CAPACITY and at that point why even bother with this at all?
	//
	// Is all of this a little too cute? Maybe this isn't worth bothering with?
	// The following reads so incredibly clear:
	// 		Shape shapes[64];
	// 		size_t num_shapes;
	//
	ShapeArray shapes;
	bool shape_is_overlapping[DEBUG_SHAPES_MAX];
} CollisionDebugScene;

void CollisionDebugScene_initialize(Game* game);
void CollisionDebugScene_update(Game* game);
void CollisionDebugScene_render(const Game* game);
