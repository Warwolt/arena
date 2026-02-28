#pragma once

#include "engine/engine.h"
#include "game/scene.h"

#include <stdbool.h>

typedef struct Game {
	bool should_quit;
	Scene scene;
	Engine engine;
} Game;
