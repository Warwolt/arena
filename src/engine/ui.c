#include "engine/ui.h"

#include "platform/assert.h"

#include <raylib.h>
#include <string.h>

#define DEBUG_ASSERT_IS_WITHIN_UI_FRAME() \
	DEBUG_ASSERT(g_ui.state.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);

UI g_ui;

void UI_initialize(void) {
	g_ui = (UI) { 0 };
}

UI* UI_get(void) {
	return &g_ui;
}

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
		DEBUG_ASSERT(!g_ui.view.menus[i].is_open, "Menu %d has missing UI_menu_end() call", i);
	}

	g_ui.state.is_within_frame = false;
}

void UI_menu_begin(const char* label) {
	DEBUG_ASSERT(g_ui.state.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);
	DEBUG_ASSERT(g_ui.view.num_menus <= 1, "FIXME: Multiple menu support not yet implemented");
	UIMenu* menu = &g_ui.view.menus[g_ui.view.num_menus++];
	menu->is_open = true;
	strncpy_s(menu->label, UIMenu_MaxLabelLength, label, _TRUNCATE);
}

void UI_menu_end(void) {
	UIMenu* menu = UI_current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_menu_end() called without UI_menu_begin()");
	DEBUG_ASSERT(menu->is_open, "UI_menu_end() called without UI_menu_begin()");
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

		if (menu->num_items == 0) {
			g_ui.state.focused_item = 0;
		} else {
			g_ui.state.focused_item = (menu->num_items + g_ui.state.focused_item + focus_delta) % menu->num_items;
		}
	}
}

bool UI_menu_item(const char* label) {
	UIMenu* menu = UI_current_menu();
	DEBUG_ASSERT(menu != NULL, "UI_menu_item() called without UI_menu_begin()");
	DEBUG_ASSERT(menu->is_open, "UI_menu_item() called without UI_menu_begin()");

	const int current_item_index = menu->num_items++;
	UIMenuItem* item = &menu->items[current_item_index];
	strncpy_s(item->label, UIMenu_MaxLabelLength, label, _TRUNCATE);

	item->is_focused = g_ui.state.focused_item == current_item_index;

	const bool is_selected = IsKeyPressed(KEY_ENTER) && item->is_focused;
	return is_selected;
}
