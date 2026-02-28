#pragma once

#include "game/scene.h"

#include <stdbool.h>

typedef struct GameState {
	SceneID scene_id;
	bool should_quit;
} GameState;
