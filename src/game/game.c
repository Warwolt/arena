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
