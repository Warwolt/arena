#include "game.h"

#include "engine/ui.h"
#include "platform/logging.h"
#include "platform/win32.h"

#include <raylib.h>
#include <raymath.h>

#include <stdio.h>
#include <stdlib.h>

// low resolution 16:9
#define SCREEN_WIDTH (int)768
#define SCREEN_HEIGHT (int)432
#define SCREEN_TITLE "Program"

void Game_initialize(Game* game, int argc, char** argv) {
	/* Initialize systems */
	initialize_logging();
	Raylib_SetTraceLogLevel(LOG_WARNING); // disable verbose raylib output
	Raylib_InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
	Raylib_SetExitKey(KEY_NULL);
	Raylib_SetTargetFPS(120);

	/* Parse command line */
	SceneID start_scene = SceneID_MainMenu;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--debug") == 0) {
			start_scene = SceneID_CollisionDebugScene;
		} else {
			LOG_ERROR("Unrecognized command: %s", argv[i]);
		}
	}

	/* Initialize game */
	const char* system_font_path = "resource/font/ModernDOS8x16.ttf";
	*game = (Game) {
		.screen = Raylib_LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT),
		.system_font = Raylib_LoadFont(system_font_path),
	};

	/* Load start scene */
	Game_switch_scene(game, start_scene);

	/* Check font */
	if (game->system_font.glyphs == NULL) {
		LOG_ERROR("Couldn't load font %s", system_font_path);
	}

	LOG_INFO("Game initialized");
}

void Game_shutdown(Game* game) {
	Raylib_UnloadFont(game->system_font);
	ResourceManager_unload_resources(&game->resources);
	Raylib_CloseWindow();
	LOG_INFO("Game shutdown");
}

void Game_update(Game* game) {
	game->should_quit = Raylib_WindowShouldClose();

	if (Raylib_IsKeyPressed(KEY_F11)) {
		Window_toggle_fullscreen(&game->window);
	}

	if (Raylib_IsKeyPressed(KEY_F3)) {
		game->show_debug_overlay = !game->show_debug_overlay;
	}

	UIInput input = {
		.up_pressed = IsKeyPressed(KEY_UP),
		.down_pressed = IsKeyPressed(KEY_DOWN),
		.select_pressed = IsKeyPressed(KEY_ENTER),
	};
	UI_begin(input);
	Scene_update(game);
	UI_end();
}

void Game_render(const Game* game) {
	/* Render onto screen texture */
	Raylib_BeginTextureMode(game->screen);
	{
		Scene_render(game);

		/* Render debug overlay */
		if (game->show_debug_overlay) {
			int font_size = 16;
			int row = 0;
			char text[128] = { 0 };

			snprintf(text, sizeof(text), "FPS: %d", GetFPS());
			Game_draw_text(game, text, 1, row++ * font_size, font_size, WHITE);

			snprintf(text, sizeof(text), "Entities: %zu", game->entities.entities.num_values);
			Game_draw_text(game, text, 1, row++ * font_size, font_size, WHITE);
		}
	}
	Raylib_EndTextureMode();

	/* Render window */
	Raylib_BeginDrawing();
	{
		// Draw background
		Raylib_ClearBackground(BLACK);

		// Draw stretched screen texture
		Rectangle screen = Game_screen_rect(game);
		int window_width = Raylib_GetScreenWidth();
		int window_height = Raylib_GetScreenHeight();
		int scale = min(window_width / screen.width, window_height / screen.height);
		int scaled_width = scale * screen.width;
		int scaled_height = scale * screen.height;
		Rectangle scaled_screen_rect = {
			.x = (window_width - scaled_width) / 2,
			.y = (window_height - scaled_height) / 2,
			.width = scaled_width,
			.height = scaled_height,
		};
		Raylib_DrawTexturePro(game->screen.texture, (Rectangle) { 0, 0, screen.width, -screen.height }, scaled_screen_rect, Vector2Zero(), 0, WHITE);
	}
	Raylib_EndDrawing();
}

void Game_quit(Game* game) {
	game->should_quit = true;
}

void Game_switch_scene(Game* game, SceneID scene_id) {
	/* Unload resources used by current scene */
	game->entities = (EntityManager) { 0 };
	ResourceManager_unload_resources(&game->resources);

	/* Initialize scene */
	Scene_initialize(game, scene_id);
}

void Game_draw_text(const Game* game, const char* text, int x, int y, int font_size, Color color) {
	Raylib_DrawTextEx(game->system_font, text, (Vector2) { x, y }, font_size, 0, color);
}

int Game_measure_text_width(const Game* game, const char* text, int font_size) {
	return Raylib_MeasureTextEx(game->system_font, text, font_size, 0).x;
}

Rectangle Game_screen_rect(const Game* game) {
	return (Rectangle) {
		.x = 0,
		.y = 0,
		.width = game->screen.texture.width,
		.height = game->screen.texture.height,
	};
}
