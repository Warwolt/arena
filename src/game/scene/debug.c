#include "game/scene/debug.h"

#include "game.h"

#include <raylib.h>

#define MIDNIGHT_COMMANDER CLITERAL(Color) { 0, 0, 187, 255 }

#define max(a, b) ((a) > (b) ? (a) : (b))

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	if (IsKeyDown(KEY_ESCAPE)) {
		Game_quit(game);
	}
}

void DebugScene_render(const Game* game) {
	ClearBackground(MIDNIGHT_COMMANDER);

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
	DrawRectangleLinesEx(outline, 2, YELLOW);

	// draw title
	{
		const char* text = "Debug Screen";
		int text_width = Game_measure_text_width(game, text, font_size);
		int text_x = (screen_rect.width - text_width) / 2;
		int text_y = 0;
		DrawRectangle(text_x - 4, text_y, text_width + 8, font_size, MIDNIGHT_COMMANDER);
		Game_draw_text(game, text, text_x, text_y, font_size, YELLOW);
	}

	// draw menu items
	{
		const char* menu_items[3] = {
			"Physics",
			"Item 2",
			"Item 3",
		};
		int num_menu_items = sizeof(menu_items) / sizeof(menu_items[0]);
		int list_height = num_menu_items * font_size;

		int list_width = 0;
		for (int i = 0; i < num_menu_items; i++) {
			int text_width = Game_measure_text_width(game, menu_items[i], font_size);
			list_width = max(list_width, text_width);
		}

		for (int i = 0; i < num_menu_items; i++) {
			const char* text = menu_items[i];
			int text_x = (screen_rect.width - list_width) / 2;
			int text_y = (screen_rect.height - list_height) / 2 + i * font_size;
			bool is_selected = i == 0;
			Color background_color = is_selected ? WHITE : MIDNIGHT_COMMANDER;
			Color text_color = is_selected ? MIDNIGHT_COMMANDER : WHITE;
			DrawRectangle(text_x - 1, text_y, list_width + 1, font_size, background_color);
			Game_draw_text(game, text, text_x, text_y, font_size, text_color);
		}
	}
}
