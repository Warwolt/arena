#include "component.h"
#include "entity.h"
#include "game.h"
#include "logging.h"
#include "resource.h"
#include "scene.h"
#include "sparse_array.h"
#include "win32.h"

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

	// Render texture
	RenderTexture2D screen_texture = LoadRenderTexture(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

	/* Load resources */
	ResourceManager resources = { 0 };

	/* State */
	GameState game = { .scene_id = SceneID_MainMenu };

	/* Run program */
	while (!game.should_quit) {
		if (IsKeyPressed(KEY_F11)) {
			toggle_fullscreen();
		}

		game.should_quit = WindowShouldClose();

		/* Update */
		{
			switch (game.scene_id) {
				case SceneID_MainMenu:
					MainMenu_update(&game);
					break;

				case SceneID_Gameplay:
					Gameplay_update(&game);
					break;
			}
		}

		/* Render scene */
		BeginTextureMode(screen_texture);
		{
			switch (game.scene_id) {
				case SceneID_MainMenu:
					MainMenu_render(&game);
					break;

				case SceneID_Gameplay:
					Gameplay_render(&game);
					break;
			}
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
			int scaled_width = scale * screen_texture.texture.width;
			int scaled_height = scale * screen_texture.texture.height;
			Rectangle scaled_screen_rect = {
				.x = (screen_width - scaled_width) / 2,
				.y = (screen_height - scaled_height) / 2,
				.width = scaled_width,
				.height = scaled_height,
			};
			DrawTexturePro(
				screen_texture.texture,
				(Rectangle) { 0, 0, screen_texture.texture.width, -screen_texture.texture.height },
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
	ResourceManager_unload_resources(&resources);
	CloseWindow();
	return 0;
}
