#include "game/scene/main_menu.h"

#include "game.h"
#include "platform/logging.h"

#include <raylib.h>
#include <raymath.h>

typedef enum MenuItem {
	MenuItem_Play,
	MenuItem_Quit,
	MenuItem_Count,
} MenuItem;

static void draw_text_centered_horizontally(const Game* game, const char* text, int font_size, int y) {
	int text_width = Game_measure_text_width(game, text, font_size);
	Rectangle window = Window_rectangle(&game->window);
	Vector2 position = {
		.x = (window.width - text_width) / 2,
		.y = y,
	};
	Game_draw_text(game, text, position.x, position.y, font_size, WHITE);
}

void MainMenu_initialize(Game* game) {
}

void MainMenu_update(Game* game) {
	MainMenu* main_menu = &game->scene.main_menu;

	if (game->input.action_is_pressed[InputAction_Select]) {
		switch (main_menu->focused_menu_item) {
			case MenuItem_Play:
				Game_switch_scene(game, SceneID_Gameplay);
				break;

			case MenuItem_Quit:
				Game_quit(game);
				break;
		}
	}

	if (game->input.action_is_pressed[InputAction_Back]) {
		game->should_quit = true;
	}

	if (game->input.action_is_pressed[InputAction_Up]) {
		main_menu->focused_menu_item = (MenuItem_Count + main_menu->focused_menu_item - 1) % MenuItem_Count;
	}

	if (game->input.action_is_pressed[InputAction_Down]) {
		main_menu->focused_menu_item = (MenuItem_Count + main_menu->focused_menu_item + 1) % MenuItem_Count;
	}
}

void MainMenu_render(const Game* game) {
	Raylib_ClearBackground(BLACK);
	const MainMenu* main_menu = &game->scene.main_menu;
	const int big_font_size = 64;
	const int small_font_size = 48;

	Rectangle window = Window_rectangle(&game->window);
	draw_text_centered_horizontally(game, "Game", big_font_size, big_font_size + big_font_size);
	draw_text_centered_horizontally(game, "Play", small_font_size, window.height / 2);
	draw_text_centered_horizontally(game, "Quit", small_font_size, window.height / 2 + small_font_size);

	Vector2 cursor_pos = {
		.x = window.width / 2 - 80,
		.y = window.height / 2 + main_menu->focused_menu_item * small_font_size,
	};
	Game_draw_text(game, ">", cursor_pos.x, cursor_pos.y, small_font_size, WHITE);
}
