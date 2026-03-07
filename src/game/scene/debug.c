#include "game/scene/debug.h"

#include "game.h"
#include "platform/assert.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

#define DEBUG_ASSERT_IS_WITHIN_UI_FRAME() \
	DEBUG_ASSERT(g_ui.state.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);

#define UIMenu_MaxMenus 4
#define UIMenu_MaxMenuItems 16
#define UIMenu_MaxLabelLength 256

typedef struct UIMenuItem {
	char label[UIMenu_MaxLabelLength];
	bool is_focused;
} UIMenuItem;

typedef struct UIMenu {
	UIMenuItem items[UIMenu_MaxMenuItems];
	char label[UIMenu_MaxLabelLength];
	int num_items;
	int focused_item;
	bool is_open;
} UIMenu;

typedef struct UIView {
	UIMenu menus[UIMenu_MaxMenus];
	int num_menus;
} UIView;

typedef struct UIState {
	bool is_within_frame;
	int focused_item;
} UIState;

// ui.view.menus
// ui.state.is_within_frame

typedef struct UI {
	UIView view;
	UIState state;
} UI;

UI g_ui;

static UIMenu* UI_current_menu() {
	if (g_ui.view.num_menus == 0) {
		return NULL;
	}
	return &g_ui.view.menus[g_ui.view.num_menus - 1];
}

void UI_begin(void) {
	DEBUG_ASSERT(!g_ui.state.is_within_frame, "%s() called while in ui frame. Missing call to UI_end()?", __FUNCTION__);
	g_ui.view = (UIView) { 0 };
	g_ui.state.is_within_frame = true;
}

void UI_end(void) {
	/* Check that each menu component is closed */
	for (int i = 0; i < g_ui.view.num_menus; i++) {
		DEBUG_ASSERT(!g_ui.view.menus[i].is_open, "Menu %d has missing UI_end_menu() call", i);
	}

	g_ui.state.is_within_frame = false;
}

void UI_begin_menu(const char* label) {
	DEBUG_ASSERT(g_ui.state.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);
	UIMenu* menu = &g_ui.view.menus[g_ui.view.num_menus++];
	menu->is_open = true;
	strncpy_s(menu->label, UIMenu_MaxLabelLength, label, _TRUNCATE);
}

void UI_end_menu(void) {
	UIMenu* menu = UI_current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_end_menu() called without UI_begin_menu()");
	DEBUG_ASSERT(menu->is_open, "UI_end_menu() called without UI_begin_menu()");
	menu->is_open = false;

	/* Handle item focus*/
	{
		int focus_delta = 0;

		if (IsKeyPressed(KEY_DOWN)) {
			focus_delta = 1;
		}

		if (IsKeyPressed(KEY_UP)) {
			focus_delta = -1;
		}

		g_ui.state.focused_item = (menu->num_items + g_ui.state.focused_item + focus_delta) % menu->num_items;
	}
}

bool UI_menu_item(const char* label) {
	UIMenu* menu = UI_current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_menu_item() called without UI_begin_menu()");
	DEBUG_ASSERT(menu->is_open, "UI_menu_item() called without UI_begin_menu()");

	const int current_item_index = menu->num_items++;
	UIMenuItem* item = &menu->items[current_item_index];
	strncpy_s(item->label, UIMenu_MaxLabelLength, label, _TRUNCATE);

	item->is_focused = g_ui.state.focused_item == current_item_index;

	const bool is_selected = IsKeyPressed(KEY_ENTER) && item->is_focused;
	return is_selected;
}

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	// testing
	UI_begin();
	{
		UI_begin_menu("Debug");
		{
			if (UI_menu_item("Item one")) {
				LOG_DEBUG("Item one pressed");
			}

			if (UI_menu_item("Item two")) {
				LOG_DEBUG("Item two pressed");
			}

			if (UI_menu_item("Item three")) {
				LOG_DEBUG("Item three pressed");
			}

			if (UI_menu_item("Item four")) {
				LOG_DEBUG("Item four pressed");
			}
		}
		UI_end_menu();
	}
	UI_end();

	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}
}

void DebugScene_render(const Game* game) {
	const DebugScene* debug_scene = &game->scene.debug_scene;
	const Rectangle screen_rect = Game_screen_rect(game);
	const int font_size = 32;

	ClearBackground(BLACK);

	/* Draw each menu */
	for (int i = 0; i < g_ui.view.num_menus; i++) {
		const UIMenu* menu = &g_ui.view.menus[i];

		/* Menu dimensions */
		int menu_width = 0;
		for (int j = 0; j < menu->num_items; j++) {
			const UIMenuItem* item = &g_ui.view.menus[i].items[j];
			int item_width = Game_measure_text_width(game, item->label, font_size);
			menu_width = max(menu_width, item_width);
		}
		const int menu_height = menu->num_items * font_size;

		/* Draw menu title */
		int menu_label_width = Game_measure_text_width(game, menu->label, font_size);
		Game_draw_text(game, menu->label, (screen_rect.width - menu_label_width) / 2, 2 * font_size, font_size, WHITE);

		/* Draw menu items */
		for (int j = 0; j < menu->num_items; j++) {
			const UIMenuItem* item = &g_ui.view.menus[i].items[j];
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
