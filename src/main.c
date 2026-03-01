#include "core/sparse_array.h"
#include "engine/component.h"
#include "engine/entity.h"
#include "engine/resource.h"
#include "game.h"
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

int main(void) {
	/* State */
	Game game = { 0 };
	Game_initialize(&game);

	/* Run program */
	while (!game.should_quit) {
		Game_update(&game);
		Game_render(&game);
	}

	/* Shutdown */
	Game_shutdown(&game);
	return 0;
}
