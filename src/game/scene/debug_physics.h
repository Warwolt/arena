#pragma once

typedef struct Game Game;

typedef struct DebugPhysics {
	int dummy;
} DebugPhysics;

void DebugPhysics_initialize(Game* game);
void DebugPhysics_update(Game* game);
void DebugPhysics_render(const Game* game);
