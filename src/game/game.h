#pragma once

#include "engine/engine.h"
#include "game/scene.h"

#include <stdbool.h>

typedef struct GameState {
	SceneID scene_id;
	bool should_quit;
	EngineState engine;
} GameState;
