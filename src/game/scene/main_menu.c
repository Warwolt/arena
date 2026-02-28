#include "game/scene/main_menu.h"

#include "game/game.h"

#include <raylib.h>
#include <raymath.h>

void MainMenu_update(Game* game) {
	if (IsKeyPressed(KEY_ENTER)) {
		Game_switch_scene(game, SceneID_Gameplay);
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}
}

void MainMenu_render(const Game* game) {
	ClearBackground(PURPLE);
	const int font_size = 64;
	const char* text = "Main Menu";
	int text_width = MeasureText(text, font_size);
	DrawText(text, (game->screen.width - text_width) / 2, (game->screen.height - font_size) / 2, font_size, WHITE);
}
