#include "game/scene/debug.h"

#include "game.h"
#include "platform/assert.h"

#include <raylib.h>
#include <stdio.h>

// int main(void) {
// 	UI_begin();
// 	{
// 		UI_begin_menu("Main menu", state->current_item == MenuType_MainMenu);
// 		{
// 			if (UI_menu_item("First item")) {
// 				LOGI("First item selected");
// 				state->current_item = MenuType_SubMenu;
// 			}

// 			if (UI_menu_item("Second item")) {
// 				LOGI("Second item selected");
// 			}

// 			if (UI_menu_item("Third item")) {
// 				LOGI("Third item selected");
// 			}
// 		}
// 		UI_end_menu();

// 		UI_begin_menu("Sub menu", state->current_item == MenuType_SubMenu);
// 		{
// 			if (UI_menu_item("First sub item")) {
// 				LOGI("First sub item selected");
// 			}

// 			if (UI_menu_item("Second sub item")) {
// 				LOGI("Second sub item selected");
// 			}

// 			if (UI_menu_item("Third sub item")) {
// 				LOGI("Third sub item selected");
// 			}
// 		}
// 		UI_end_menu();
// 	}
// 	UI_end();
// }

#define max(a, b) ((a) > (b) ? (a) : (b))

#define DEBUG_ASSERT_IS_WITHIN_UI_FRAME()                                                                          \
	DEBUG_ASSERT(g_ui.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);

#define UIMenu_MaxMenuItems 16
#define UIMenu_MaxMenus 4

typedef struct UIMenuItem {
	const char* label;
} UIMenuItem;

typedef struct UIMenu {
	UIMenuItem items[UIMenu_MaxMenuItems];
	int num_items;
	int focused_item;
	bool is_open;
} UIMenu;

typedef struct UI {
	bool is_within_frame;
	UIMenu menus[UIMenu_MaxMenus];
	int num_menus;
} UI;

UI g_ui;

static UIMenu* current_menu() {
	if (g_ui.num_menus == 0) {
		return NULL;
	}
	return &g_ui.menus[g_ui.num_menus - 1];
}

void UI_begin(void) {
	// start UI frame
	// I guess we'd want to check all inputs here any store some kind of input state if we need?
	DEBUG_ASSERT(!g_ui.is_within_frame, "%s() called while in ui frame. Missing call to UI_end()?", __FUNCTION__);
	g_ui = (UI) { 0 };
	g_ui.is_within_frame = true;
}

void UI_end(void) {
	/* Check that each menu component is closed */
	for (int i = 0; i < g_ui.num_menus; i++) {
		DEBUG_ASSERT(!g_ui.menus[i].is_open, "Menu %d has missing UI_end_menu() call", i);
	}

	g_ui.is_within_frame = false;
}

// let user pass in focused, since what menu is focused depends on the semantics of the specific
// menu and how it's interaction handlers are defined in user code.
void UI_begin_menu() {
	DEBUG_ASSERT(g_ui.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);
	g_ui.num_menus++;
	g_ui.menus[g_ui.num_menus - 1].is_open = true;
}

void UI_end_menu(void) {
	UIMenu* menu = current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_end_menu() called without UI_begin_menu()");
	DEBUG_ASSERT(menu->is_open, "UI_end_menu() called without UI_begin_menu()");
	menu->is_open = false;
}

bool UI_menu_item(const char* label) {
	UIMenu* menu = current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_menu_item() called without UI_begin_menu()");
	DEBUG_ASSERT(menu->is_open, "UI_menu_item() called without UI_begin_menu()");

	// FIXME: we should be copying the `label` into the menu to avoid lifetime issues.
	menu->num_items++;
	menu->items[menu->num_items - 1].label = label;

	bool is_selected = false;
	return is_selected;
}

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	// testing
	UI_begin();
	{
		UI_begin_menu();
		{
			UI_menu_item("Item one");
			UI_menu_item("Item two");
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

	for (int i = 0; i < g_ui.num_menus; i++) {
		for (int j = 0; j < g_ui.menus[i].num_items; j++) {
			const UIMenuItem* item = &g_ui.menus[i].items[j];
			Game_draw_text(game, item->label, 0, j * font_size, font_size, WHITE);
		}
	}
}
