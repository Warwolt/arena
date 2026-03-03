#include "game/scene/debug.h"

#include "game.h"

#include <raylib.h>

#define MIDNIGHT_COMMANDER CLITERAL(Color) { 0, 0, 187, 255 }

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum DebugMenuItem {
	DebugMenuItem_Physics,
	DebugMenuItem_ItemTwo,
	DebugMenuItem_ItemThree,
	DebugMenuItem_Count,
} DebugMenuItem;

typedef enum DebugScreen {
	DebugScreen_Menu,
	DebugScreen_Physics,
} DebugScreen;

static const char* menu_item_names[DebugMenuItem_Count] = {
	[DebugMenuItem_Physics] = "1. Physics",
	[DebugMenuItem_ItemTwo] = "2. ItemTwo",
	[DebugMenuItem_ItemThree] = "3. ItemThree",
};

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	DebugScene* debug_scene = &game->scene.debug_scene;
	switch (debug_scene->current_screen) {
		case DebugScreen_Menu: {
			if (IsKeyPressed(KEY_ESCAPE)) {
				Game_quit(game);
			}

			if (IsKeyPressed(KEY_DOWN)) {
				debug_scene->focused_menu_item = (DebugMenuItem_Count + debug_scene->focused_menu_item + 1) % DebugMenuItem_Count;
			}

			if (IsKeyPressed(KEY_UP)) {
				debug_scene->focused_menu_item = (DebugMenuItem_Count + debug_scene->focused_menu_item - 1) % DebugMenuItem_Count;
			}

			if (IsKeyPressed(KEY_ENTER)) {
				if (debug_scene->focused_menu_item == DebugMenuItem_Physics) {
					debug_scene->current_screen = DebugScreen_Physics;
				}
			}

		} break;

		case DebugScreen_Physics: {
			if (IsKeyPressed(KEY_ESCAPE)) {
				debug_scene->current_screen = DebugScreen_Menu;
			}
		} break;
	}
}

void DebugScene_render(const Game* game) {
	const DebugScene* debug_scene = &game->scene.debug_scene;
	const Rectangle screen_rect = Game_screen_rect(game);
	const int font_size = 32;

	ClearBackground(MIDNIGHT_COMMANDER);

	switch (debug_scene->current_screen) {
		case DebugScreen_Menu: {
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
				int list_height = DebugMenuItem_Count * font_size;
				int list_width = 0;
				for (int i = 0; i < DebugMenuItem_Count; i++) {
					int text_width = Game_measure_text_width(game, menu_item_names[i], font_size);
					list_width = max(list_width, text_width);
				}

				for (int i = 0; i < DebugMenuItem_Count; i++) {
					const char* text = menu_item_names[i];
					int text_x = (screen_rect.width - list_width) / 2;
					int text_y = (screen_rect.height - list_height) / 2 + i * font_size;
					bool is_focused = i == debug_scene->focused_menu_item;
					Color background_color = is_focused ? WHITE : MIDNIGHT_COMMANDER;
					Color text_color = is_focused ? MIDNIGHT_COMMANDER : WHITE;
					DrawRectangle(text_x - 1, text_y, list_width + 1, font_size, background_color);
					Game_draw_text(game, text, text_x, text_y, font_size, text_color);
				}
			}
		} break;

		case DebugScreen_Physics: {
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
				const char* text = "Physics";
				int text_width = Game_measure_text_width(game, text, font_size);
				int text_x = (screen_rect.width - text_width) / 2;
				int text_y = 0;
				DrawRectangle(text_x - 4, text_y, text_width + 8, font_size, MIDNIGHT_COMMANDER);
				Game_draw_text(game, text, text_x, text_y, font_size, YELLOW);
			}
		} break;
	}
}
