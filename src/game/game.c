#include "game/game.h"

#include "platform/window.h"

#include <raylib.h>

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
	switch (game->scene.id) {
		case SceneID_MainMenu:
			MainMenu_render(game);
			break;

		case SceneID_Gameplay:
			Gameplay_render(game);
			break;
	}
}

void Game_quit(Game* game) {
	game->should_quit = true;
}

void Game_switch_scene(Game* game, SceneID id) {
	game->scene = (Scene) { .id = id };
}

Rectangle Game_screen_rect(const Game* game) {
	return (Rectangle) {
		.x = 0,
		.y = 0,
		.width = game->screen.texture.width,
		.height = game->screen.texture.height,
	};
}
