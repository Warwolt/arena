#include "engine/ui.h"

#include "platform/assert.h"

#include <raylib.h>
#include <string.h>

#define DEBUG_ASSERT_IS_WITHIN_UI_FRAME()                                                                          \
	DEBUG_ASSERT(g_ui.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);

// TODO:
// Add a UIMenuState struct that hold stuff that stays between frames
// But, we would like a hashmap like data structure to store it.
// We need to be able to map MenuLabel -> MenuState.
// Probably we need an `ArrayMap` data structure that maps `String -> T`.

typedef struct UIContext {
	bool is_within_frame;
	UIInput input;
	UIView view;
	UIView prev_view;
} UIContext;

UIContext g_ui;

void UI_initialize(void) {
	g_ui = (UIContext) { 0 };
}

const UIView* UI_view(void) {
	return &g_ui.view;
}

static UIMenu* UI_current_menu() {
	if (g_ui.view.num_menus == 0) {
		return NULL;
	}
	return &g_ui.view.menus[g_ui.view.num_menus - 1];
}

void UI_begin(UIInput input) {
	DEBUG_ASSERT(!g_ui.is_within_frame, "UI_begin() called while already in ui frame. Missing call to UI_end()?");
	g_ui.input = input;
	g_ui.is_within_frame = true;
	g_ui.prev_view = g_ui.view;
	g_ui.view = (UIView) { 0 };
}

void UI_end(void) {
	DEBUG_ASSERT(g_ui.is_within_frame, "UI_end() called while outside ui frame. Missing call to UI_begin()?");

	/* Check that each menu component is closed */
	for (int i = 0; i < g_ui.view.num_menus; i++) {
		UIMenu* menu = &g_ui.view.menus[i];
		DEBUG_ASSERT(!menu->is_open, "Menu \"%s\" has missing UI_menu_end() call", menu->label);
	}

	g_ui.is_within_frame = false;
}

void UI_menu_begin(const char* label) {
	/* Check that we're not nesting menus */
	{
		UIMenu* menu = UI_current_menu();
		DEBUG_ASSERT_IS_WITHIN_UI_FRAME();
		DEBUG_ASSERT(menu == NULL || !menu->is_open, "UI_menu_begin() called while menu \"%s\" already open. Menus cannot be nested. Missing call to UI_menu_end()?", menu->label);
	}

	/* Add menu */
	const int menu_index = g_ui.view.num_menus++;
	UIMenu* menu = &g_ui.view.menus[menu_index];
	strncpy_s(menu->label, UIMenu_MaxLabelLength, label, _TRUNCATE);

	/* Copy previous menu state */
	// Try to find this menu in prev view
	// If it exists, copy over the state
	UIMenu* prev_menu = NULL;
	for (int i = 0; i < g_ui.prev_view.num_menus; i++) {
		if (strcmp(label, g_ui.prev_view.menus[i].label) == 0) {
			prev_menu = &g_ui.prev_view.menus[i];
			break;
		}
	}
	if (prev_menu) {
		menu->focused_item = prev_menu->focused_item;
	}

	menu->is_open = true;

	/* Handle focus */
	if (g_ui.input.down_pressed) {
		menu->focused_item++;
	}

	if (g_ui.input.up_pressed) {
		menu->focused_item--;
	}

	if (prev_menu) {
		// wrap focus around previous number of menu items
		menu->focused_item = (prev_menu->num_items + menu->focused_item) % prev_menu->num_items;
	}
}

void UI_menu_end(void) {
	UIMenu* menu = UI_current_menu();
	DEBUG_ASSERT_IS_WITHIN_UI_FRAME();
	DEBUG_ASSERT(menu != NULL, "UI_menu_end() called without corresponding UI_menu_begin()");
	DEBUG_ASSERT(menu->is_open, "UI_menu_end() called without corresponding UI_menu_begin()");
	menu->is_open = false;
}

bool UI_menu_item(const char* label) {
	UIMenu* menu = UI_current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_menu_item() called without UI_menu_begin()");
	DEBUG_ASSERT(menu->is_open, "UI_menu_item() called without UI_menu_begin()");

	/* Add item */
	const int item_index = menu->num_items++;
	UIMenuItem* item = &menu->items[item_index];
	strncpy_s(item->label, UIMenu_MaxLabelLength, label, _TRUNCATE);

	item->is_focused = menu->focused_item == item_index;

	/* Handle selection */
	const bool is_selected = g_ui.input.select_pressed && item->is_focused;
	return is_selected;
}
