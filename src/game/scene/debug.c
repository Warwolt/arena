#include "game/scene/debug.h"

#include "game.h"

#include <raylib.h>

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	if (IsKeyDown(KEY_ESCAPE)) {
		Game_quit(game);
	}
}

void DebugScene_render(const Game* game) {
	Color midnight_commander = { .r = 0, .g = 0, .b = 187, .a = 255 };
	ClearBackground(midnight_commander);

	const Rectangle screen_rect = Game_screen_rect(game);
	const int font_size = 32;

	// draw outline
	const int x_off = font_size / 4;
	const int y_off = font_size / 2;
	const Rectangle outline = {
		.x = x_off,
		.y = y_off,
		.width = screen_rect.width - 2 * x_off,
		.height = screen_rect.height - 2 * y_off,
	};
	DrawRectangleLinesEx(outline, 1, YELLOW);

	// draw title
	{
		const char* text = "Debug Screen";
		int text_width = Game_measure_text_width(game, text, font_size);
		int text_x = (screen_rect.width - text_width) / 2;
		int text_y = 0;
		DrawRectangle(text_x, text_y, text_width, font_size, midnight_commander);
		Game_draw_text(game, text, text_x, text_y, font_size, YELLOW);
	}

	// draw menu items
	{
		const char* text = "Physics";
		int text_width = Game_measure_text_width(game, text, font_size);
		int text_x = (screen_rect.width - text_width) / 2;
		int text_y = 3 * font_size;
		Game_draw_text(game, text, text_x, text_y, font_size, WHITE);
	}
}
