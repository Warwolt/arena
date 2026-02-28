#include "core/sparse_array.h"
#include "engine/component.h"
#include "engine/entity.h"
#include "engine/resource.h"
#include "game/game.h"
#include "game/scene/gameplay.h"
#include "game/scene/main_menu.h"
#include "platform/logging.h"

#include "raylib_extra.h"
#include <raylib.h>
#include <raymath.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// low resolution 16:9
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

int main(void) {
	/* Init */
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "Program");
	SetExitKey(KEY_NULL);
	LOG_INFO("Created window");

	/* State */
	Game game = { 0 };
	Game_initialize(&game, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

	/* Run program */
	while (!game.should_quit) {
		Game_update(&game);
		Game_render(&game);
	}

	/* Shutdown */
	LOG_INFO("Shutdown");
	ResourceManager_unload_resources(&game.resources);
	CloseWindow();
	return 0;
}
