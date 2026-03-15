#include "game/scene/debug.h"

#include "engine/ui.h"
#include "game.h"
#include "platform/assert.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

typedef enum DebugMenu {
	DebugMenu_Main,
	DebugMenu_Input,
	DebugMenu_Physics,
} DebugMenu;

void MenuStack_push_menu(MenuStack* stack, int menu) {
	ASSERT(stack->num_menus != MenuStack_MaxDepth, "%s called with max num menus pushed", __FUNCTION__);
	stack->menus[stack->num_menus++] = menu;
}

void MenuStack_pop_menu(MenuStack* stack) {
	stack->num_menus = max(stack->num_menus - 1, 0);
}

int MenuStack_current_menu(MenuStack* stack) {
	if (stack->num_menus > 0) {
		return stack->menus[stack->num_menus - 1];
	}
	return 0;
}

void MenuStack_update(MenuStack* stack) {
	stack->has_pushed_menu = stack->prev_num_menus < stack->num_menus;
	stack->prev_num_menus = stack->num_menus;
}

void DebugScene_initialize(Game* game) {
	DebugScene* debug_scene = &game->scene.debug_scene;
	MenuStack_push_menu(&debug_scene->menu_stack, DebugMenu_Main);
}

void DebugScene_update(Game* game) {
	DebugScene* debug_scene = &game->scene.debug_scene;
	MenuStack_update(&debug_scene->menu_stack);

	// testing
	UIInput input = {
		.up_pressed = Raylib_IsKeyPressed(KEY_UP),
		.down_pressed = Raylib_IsKeyPressed(KEY_DOWN),
		.select_pressed = Raylib_IsKeyPressed(KEY_ENTER),
	};
	UI_begin(input);
	{
		if (debug_scene->menu_stack.has_pushed_menu) {
			UI_reset_next_keyboard_focus();
		}

		switch (MenuStack_current_menu(&debug_scene->menu_stack)) {
			case DebugMenu_Main:
				UI_menu_begin("Debug");
				{
					if (Raylib_IsKeyPressed(KEY_ESCAPE)) {
						Game_quit(game);
					}

					if (UI_menu_item("Input")) {
						MenuStack_push_menu(&debug_scene->menu_stack, DebugMenu_Input);
					}

					if (UI_menu_item("Physics")) {
						MenuStack_push_menu(&debug_scene->menu_stack, DebugMenu_Physics);
					}
				}
				UI_menu_end();
				break;

			case DebugMenu_Input:
				UI_menu_begin("Input Debug");
				{
					if (Raylib_IsKeyPressed(KEY_ESCAPE)) {
						MenuStack_pop_menu(&debug_scene->menu_stack);
					}

					UI_menu_item("Item 1");
					UI_menu_item("Item 2");
					UI_menu_item("Item 3");
				}
				UI_menu_end();
				break;

			case DebugMenu_Physics:
				UI_menu_begin("Physics Debug");
				{
					if (Raylib_IsKeyPressed(KEY_ESCAPE)) {
						MenuStack_pop_menu(&debug_scene->menu_stack);
					}

					UI_menu_item("Item 1");
					UI_menu_item("Item 2");
					UI_menu_item("Item 3");
				}
				UI_menu_end();
				break;
		}
	}
	UI_end();
}

void DebugScene_render(const Game* game) {
	const DebugScene* debug_scene = &game->scene.debug_scene;
	const Rectangle window = Window_rectangle(&game->window);
	const int font_size = 32;

	Raylib_ClearBackground(BLACK);

	/* Draw each menu */
	for (int i = 0; i < UI_view()->num_menus; i++) {
		const UIMenu* menu = &UI_view()->menus[i];

		/* Menu dimensions */
		int menu_width = 0;
		for (int j = 0; j < menu->num_items; j++) {
			const UIMenuItem* item = &UI_view()->menus[i].items[j];
			int item_width = Game_measure_text_width(game, item->label, font_size);
			menu_width = max(menu_width, item_width);
		}
		const int menu_height = menu->num_items * font_size;

		/* Draw menu title */
		int menu_label_width = Game_measure_text_width(game, menu->label, font_size);
		Game_draw_text(game, menu->label, (window.width - menu_label_width) / 2, 2 * font_size, font_size, WHITE);

		/* Draw menu items */
		for (int j = 0; j < menu->num_items; j++) {
			const UIMenuItem* item = &UI_view()->menus[i].items[j];
			const int margin_left = (window.width - menu_width) / 2;
			const int margin_top = (window.height - menu_height) / 2;
			const int pos_x = margin_left;
			const int pos_y = margin_top + j * font_size;
			if (item->is_focused) {
				Raylib_DrawRectangle(pos_x, pos_y, menu_width, font_size, WHITE);
				Game_draw_text(game, item->label, pos_x, pos_y, font_size, BLACK);
			} else {
				Game_draw_text(game, item->label, pos_x, pos_y, font_size, WHITE);
			}
		}
	}
}
