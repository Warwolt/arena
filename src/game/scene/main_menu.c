#include "game/scene/main_menu.h"

#include "game/game.h"

#include <raylib.h>
#include <raymath.h>

void draw_centered_horizontally(const Game* game, const char* text, int font_size, int y) {
	int text_width = MeasureText(text, font_size);
	Rectangle screen = Game_screen_rect(game);
	Vector2 position = {
		.x = (screen.width - text_width) / 2,
		.y = y,
	};
	DrawText(text, position.x, position.y, font_size, WHITE);
}

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
	const int big_font_size = 64;
	const int small_font_size = 48;

	Rectangle screen = Game_screen_rect(game);
	draw_centered_horizontally(game, "Main Menu", big_font_size, big_font_size + big_font_size);
	draw_centered_horizontally(game, "Play", small_font_size, screen.height / 2);
	draw_centered_horizontally(game, "Quit", small_font_size, screen.height / 2 + small_font_size);

	DrawText(">", screen.width / 2 - 80, screen.height / 2, small_font_size, WHITE);
}
