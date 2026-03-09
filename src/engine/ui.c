#include "engine/ui.h"

#include "platform/assert.h"

#include <raylib.h>
#include <string.h>

#define DEBUG_ASSERT_IS_WITHIN_UI_FRAME() \
	DEBUG_ASSERT(g_ui.state.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);

typedef struct UIState {
	bool is_within_frame;
	int focused_item;
} UIState;

typedef struct UIContext {
	UIInput input;
	UIState state;
	UIView view;
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
	DEBUG_ASSERT(!g_ui.state.is_within_frame, "UI_begin() called while already in ui frame. Missing call to UI_end()?");
	g_ui.input = input;
	g_ui.state.is_within_frame = true;
	g_ui.view = (UIView) { 0 };
}

void UI_end(void) {
	DEBUG_ASSERT(g_ui.state.is_within_frame, "UI_end() called while outside ui frame. Missing call to UI_begin()?");

	/* Check that each menu component is closed */
	for (int i = 0; i < g_ui.view.num_menus; i++) {
		UIMenu* menu = &g_ui.view.menus[i];
		DEBUG_ASSERT(!menu->is_open, "Menu \"%s\" has missing UI_menu_end() call", menu->label);
	}

	g_ui.state.is_within_frame = false;
}

void UI_menu_begin(const char* label) {
	UIMenu* prev_menu = UI_current_menu();
	DEBUG_ASSERT_IS_WITHIN_UI_FRAME();
	DEBUG_ASSERT(
		prev_menu == NULL || !prev_menu->is_open,
		"UI_menu_begin() called while menu \"%s\" already open. Menus cannot be nested. Missing call to UI_menu_end()?",
		prev_menu->label
	);

	/* Add menu */
	UIMenu* menu = &g_ui.view.menus[g_ui.view.num_menus++];
	menu->is_open = true;
	strncpy_s(menu->label, UIMenu_MaxLabelLength, label, _TRUNCATE);

	/* Handle focus */
	if (g_ui.input.down_pressed) {
		g_ui.state.focused_item++;
	}

	if (g_ui.input.up_pressed) {
		g_ui.state.focused_item--;
	}

	// FIXME: how do we handle wrapping focus item around? At `UI_menu_begin` we don't know how many
	// items we have, but we need to know which item is focused when handling each `UI_menu_item`.
	// Only way out seems to be to store the previous `view` inside UIContext, so that we have
	// memory of it when doing computations here.
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

	item->is_focused = g_ui.state.focused_item == item_index;

	/* Handle selection */
	const bool is_selected = g_ui.input.select_pressed && item->is_focused;
	return is_selected;
}
