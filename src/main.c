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

// UFO 50 is 16:9 at 384x216 resolution
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
	Game game = {
		.should_quit = false,
		.scene = { .id = SceneID_MainMenu },
		.resources = { 0 },
		.screen = LoadRenderTexture(RESOLUTION_WIDTH, RESOLUTION_HEIGHT),
	};

	/* Run program */
	while (!game.should_quit) {
		Game_update(&game);

		/* Render scene */
		BeginTextureMode(game.screen);
		{
			Game_render(&game);
		}
		EndTextureMode();

		/* Render window */
		BeginDrawing();
		{
			// Draw background
			ClearBackground(BLACK);

			// Draw stretched screen texture
			int screen_width = GetScreenWidth();
			int screen_height = GetScreenHeight();
			int scale = min(screen_width / RESOLUTION_WIDTH, screen_height / RESOLUTION_HEIGHT);
			int scaled_width = scale * game.screen.texture.width;
			int scaled_height = scale * game.screen.texture.height;
			Rectangle scaled_screen_rect = {
				.x = (screen_width - scaled_width) / 2,
				.y = (screen_height - scaled_height) / 2,
				.width = scaled_width,
				.height = scaled_height,
			};
			DrawTexturePro(
				game.screen.texture,
				(Rectangle) { 0, 0, game.screen.texture.width, -game.screen.texture.height },
				scaled_screen_rect,
				Vector2Zero(),
				0,
				WHITE
			);
		}
		EndDrawing();
	}

	/* Shutdown */
	LOG_INFO("Shutdown");
	ResourceManager_unload_resources(&game.resources);
	CloseWindow();
	return 0;
}
