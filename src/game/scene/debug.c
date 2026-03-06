#include "game/scene/debug.h"

#include "game.h"
#include "platform/assert.h"

#include <raylib.h>

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

#define UIMenu_MaxItemLabels 16
#define UIMenu_MaxMenus 4

typedef struct UIMenu {
	const char* item_labels[UIMenu_MaxItemLabels];
	int num_items;
	int focused_item;
} UIMenu;

typedef struct UI {
	bool is_within_frame;
	UIMenu menus[UIMenu_MaxMenus];
	int num_menus;
} UI;

UI g_ui_context;

UI* UI_context(void) {
	return &g_ui_context;
}

void UI_begin(void) {
	// start UI frame
	// I guess we'd want to check all inputs here any store some kind of input state if we need?
	UI* ui = UI_context();
	ui->is_within_frame = true;
}

void UI_end(void) {
	// end UI frame
	// any other UI calls except UI_begin should give an error at this point
	UI* ui = UI_context();
	ui->is_within_frame = false;
}

// let user pass in focused, since what menu is focused depends on the semantics of the specific
// menu and how it's interaction handlers are defined in user code.
void UI_begin_menu() {
	UI* ui = UI_context();
	// push a new menu to add items to
}

bool UI_menu_item(const char* label) {
	// DEBUG_ASSERT(we have an open menu component);

	// push an item to the current open menu
	// if no menu open, instead result in error
	//
	// let is_selected = menu item is focused && enter pressed
	// return is_selected
	bool is_selected = false;
	return is_selected;
}

void UI_end_menu(void) {
	// after this call, any calls to `UI_menu_item` should trigger an error
	// if this isn't called, shouldn't we error?
}

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	// testing
	{
		const bool main_menu_focused = true;
		UI_begin_menu();
		{
			//
		}
		UI_end_menu();
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}
}

void DebugScene_render(const Game* game) {
	const DebugScene* debug_scene = &game->scene.debug_scene;
	const Rectangle screen_rect = Game_screen_rect(game);
	const int font_size = 32;

	ClearBackground(BLACK);
	// render each menu
}
