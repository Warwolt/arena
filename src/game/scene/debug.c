#include "game/scene/debug.h"

#include "engine/ui.h"
#include "game.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum DebugPage {
	DebugPage_Main,
	DebugPage_PageOne,
	DebugPage_PageTwo,
} DebugPage;

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	DebugScene* debug_scene = &game->scene.debug_scene;

	// testing
	UI_begin();
	{
		switch (debug_scene->current_page) {
			case DebugPage_Main:
				UI_menu_begin("Debug");
				{
					if (IsKeyPressed(KEY_ESCAPE)) {
						Game_quit(game);
					}

					if (UI_menu_item("Page one")) {
						debug_scene->current_page = DebugPage_PageOne;
					}

					if (UI_menu_item("Page two")) {
						debug_scene->current_page = DebugPage_PageTwo;
					}
				}
				UI_menu_end();
				break;

			case DebugPage_PageOne:
				UI_menu_begin("Page One");
				{
					if (IsKeyPressed(KEY_ESCAPE)) {
						debug_scene->current_page = DebugPage_Main;
					}
				}
				UI_menu_end();
				break;

			case DebugPage_PageTwo:
				UI_menu_begin("Page Two");
				{
					if (IsKeyPressed(KEY_ESCAPE)) {
						debug_scene->current_page = DebugPage_Main;
					}
				}
				UI_menu_end();
				break;
		}
	}
	UI_end();
}

void DebugScene_render(const Game* game) {
	const DebugScene* debug_scene = &game->scene.debug_scene;
	const Rectangle screen_rect = Game_screen_rect(game);
	const int font_size = 32;

	ClearBackground(BLACK);

	/* Draw each menu */
	for (int i = 0; i < UI_get()->view.num_menus; i++) {
		const UIMenu* menu = &UI_get()->view.menus[i];

		/* Menu dimensions */
		int menu_width = 0;
		for (int j = 0; j < menu->num_items; j++) {
			const UIMenuItem* item = &UI_get()->view.menus[i].items[j];
			int item_width = Game_measure_text_width(game, item->label, font_size);
			menu_width = max(menu_width, item_width);
		}
		const int menu_height = menu->num_items * font_size;

		/* Draw menu title */
		int menu_label_width = Game_measure_text_width(game, menu->label, font_size);
		Game_draw_text(game, menu->label, (screen_rect.width - menu_label_width) / 2, 2 * font_size, font_size, WHITE);

		/* Draw menu items */
		for (int j = 0; j < menu->num_items; j++) {
			const UIMenuItem* item = &UI_get()->view.menus[i].items[j];
			const int margin_left = (screen_rect.width - menu_width) / 2;
			const int margin_top = (screen_rect.height - menu_height) / 2;
			const int pos_x = margin_left;
			const int pos_y = margin_top + j * font_size;
			if (item->is_focused) {
				DrawRectangle(pos_x, pos_y, menu_width, font_size, WHITE);
				Game_draw_text(game, item->label, pos_x, pos_y, font_size, BLACK);
			} else {
				Game_draw_text(game, item->label, pos_x, pos_y, font_size, WHITE);
			}
		}
	}
}
