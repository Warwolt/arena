#include "game/game.h"

#include "platform/window.h"

#include <raylib.h>
#include <raymath.h>

#include <stdlib.h>

void Game_initialize(Game* game, int screen_width, int screen_height) {
	*game = (Game) {
		.should_quit = false,
		.resources = { 0 },
		.screen = LoadRenderTexture(screen_width, screen_height),
	};
	Game_switch_scene(game, SceneID_MainMenu);
}

void Game_update(Game* game) {
	/* Global behavior */
	game->should_quit = WindowShouldClose();

	if (IsKeyPressed(KEY_F11)) {
		Window_toggle_fullscreen();
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
	/* Render scene */
	BeginTextureMode(game->screen);
	{
		switch (game->scene.id) {
			case SceneID_MainMenu:
				MainMenu_render(game);
				break;

			case SceneID_Gameplay:
				Gameplay_render(game);
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
