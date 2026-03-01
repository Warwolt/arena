#include "game.h"

#include "platform/logging.h"
#include "platform/window.h"

#include <raylib.h>
#include <raymath.h>

#include <stdio.h>
#include <stdlib.h>

// low resolution 16:9
#define SCREEN_WIDTH (int)768
#define SCREEN_HEIGHT (int)432

void Game_initialize(Game* game) {
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Program");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	const SceneID start_scene = SceneID_MainMenu;
	*game = (Game) {
		.should_quit = false,
		.resources = { 0 },
		.screen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT),
	};
	Game_switch_scene(game, start_scene);

	LOG_INFO("Game initialized");
}

void Game_shutdown(Game* game) {
	LOG_INFO("Game shutdown");
	ResourceManager_unload_resources(&game->resources);
	CloseWindow();
}

void Game_update(Game* game) {
	/* Global behavior */
	game->should_quit = WindowShouldClose();

	if (IsKeyPressed(KEY_F11)) {
		Window_toggle_fullscreen();
	}

	if (IsKeyPressed(KEY_F3)) {
		game->show_debug_overlay = !game->show_debug_overlay;
	}

	/* Update scene */
	switch (game->scene.id) {
		case SceneID_MainMenu:
			MainMenu_update(game);
			break;

		case SceneID_Gameplay:
			Gameplay_update(game);
			break;
	}
}

void Game_render(const Game* game) {
	/* Render onto screen texture */
	BeginTextureMode(game->screen);
	{
		/* Render scene */
		switch (game->scene.id) {
			case SceneID_MainMenu:
				MainMenu_render(game);
				break;

			case SceneID_Gameplay:
				Gameplay_render(game);
				break;
		}

		/* Render debug overlay */
		if (game->show_debug_overlay) {
			int font_size = 24;
			int row = 0;
			char text[128] = { 0 };

			snprintf(text, sizeof(text), "FPS: %d", GetFPS());
			DrawText(text, 1, row++ * font_size, font_size, WHITE);

			snprintf(text, sizeof(text), "Entities: %zu", game->entities.entities.size);
			DrawText(text, 1, row++ * font_size, font_size, WHITE);
		}
	}
	EndTextureMode();

	/* Render window */
	BeginDrawing();
	{
		// Draw background
		ClearBackground(BLACK);

		// Draw stretched screen texture
		Rectangle screen = Game_screen_rect(game);
		int window_width = GetScreenWidth();
		int window_height = GetScreenHeight();
		int scale = min(window_width / screen.width, window_height / screen.height);
		int scaled_width = scale * screen.width;
		int scaled_height = scale * screen.height;
		Rectangle scaled_screen_rect = {
			.x = (window_width - scaled_width) / 2,
			.y = (window_height - scaled_height) / 2,
			.width = scaled_width,
			.height = scaled_height,
		};
		DrawTexturePro(game->screen.texture, (Rectangle) { 0, 0, screen.width, -screen.height }, scaled_screen_rect, Vector2Zero(), 0, WHITE);
	}
	EndDrawing();
}

void Game_quit(Game* game) {
	game->should_quit = true;
}

void Game_switch_scene(Game* game, SceneID id) {
	game->scene = (Scene) { .id = id };
	game->entities = (EntityManager) { 0 };
	ResourceManager_unload_resources(&game->resources);
	switch (id) {
		case SceneID_MainMenu:
			MainMenu_initialize(game);
			break;

		case SceneID_Gameplay:
			Gameplay_initialize(game);
			break;
	}
}

Rectangle Game_screen_rect(const Game* game) {
	return (Rectangle) {
		.x = 0,
		.y = 0,
		.width = game->screen.texture.width,
		.height = game->screen.texture.height,
	};
}
