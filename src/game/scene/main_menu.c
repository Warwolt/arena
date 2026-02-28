#include "game/scene/main_menu.h"

#include "game/game.h"

#include <raylib.h>
#include <raymath.h>

#define NUM_MENU_ITEMS 2

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
	MainMenu* main_menu = &game->scene.main_menu;

	if (IsKeyPressed(KEY_ENTER)) {
		Game_switch_scene(game, SceneID_Gameplay);
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}

	if (IsKeyPressed(KEY_DOWN)) {
		main_menu->selected_menu_item = (NUM_MENU_ITEMS + main_menu->selected_menu_item + 1) % NUM_MENU_ITEMS;
	}

	if (IsKeyPressed(KEY_UP)) {
		main_menu->selected_menu_item = (NUM_MENU_ITEMS + main_menu->selected_menu_item - 1) % NUM_MENU_ITEMS;
	}
}

void MainMenu_render(const Game* game) {
	ClearBackground(PURPLE);
	const MainMenu* main_menu = &game->scene.main_menu;
	const int big_font_size = 64;
	const int small_font_size = 48;

	Rectangle screen = Game_screen_rect(game);
	draw_centered_horizontally(game, "Main Menu", big_font_size, big_font_size + big_font_size);
	draw_centered_horizontally(game, "Play", small_font_size, screen.height / 2);
	draw_centered_horizontally(game, "Quit", small_font_size, screen.height / 2 + small_font_size);

	Vector2 cursor_pos = {
		.x = screen.width / 2 - 80,
		.y = screen.height / 2 + main_menu->selected_menu_item * small_font_size,
	};
	DrawText(">", cursor_pos.x, cursor_pos.y, small_font_size, WHITE);
}
