#include "engine/ui.h"

#include "core/array_map.h"
#include "platform/assert.h"

#include <raylib.h>
#include <string.h>

#define DEBUG_ASSERT_IS_WITHIN_UI_FRAME()                                                                          \
	DEBUG_ASSERT(g_ui.is_within_frame, "%s() called outside ui frame. Missing call to UI_begin()?", __FUNCTION__);

typedef struct UIMenuState {
	int focused_item;
	bool element_closed;
} UIMenuState;

typedef struct UIState {
	ArrayMap(UIMenuState, 64) menu;
} UIState;

typedef struct UIContext {
	bool is_within_frame;
	UIInput input;
	UIView view;
	UIView prev_view;
	UIState state;
} UIContext;

UIContext g_ui;

void UI_initialize(void) {
	g_ui = (UIContext) { 0 };
}

const UIView* UI_view(void) {
	return &g_ui.view;
}

UIMenuState* UI_menu_state(const char* menu_label) {
	UIMenuState* state = NULL;
	ArrayMap_get_ptr(&g_ui.state.menu, menu_label, &state);
	return state; // might be null
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
		UIMenuState* menu_state = UI_menu_state(menu->label);
		DEBUG_ASSERT(menu_state->element_closed, "Menu \"%s\" has missing UI_menu_end() call", menu->label);
	}

	g_ui.is_within_frame = false;
}

void UI_menu_begin(const char* label) {
	/* Check that we're not nesting menus */
	{
		UIMenu* menu = UI_current_menu();
		DEBUG_ASSERT_IS_WITHIN_UI_FRAME();
		DEBUG_ASSERT(menu == NULL || UI_menu_state(menu->label)->element_closed, "UI_menu_begin() called while menu \"%s\" already open. Menus cannot be nested. Missing call to UI_menu_end()?", menu->label);
	}

	/* Add menu */
	const int menu_index = g_ui.view.num_menus++;
	UIMenu* menu = &g_ui.view.menus[menu_index];
	strncpy_s(menu->label, UIMenu_MaxLabelLength, label, _TRUNCATE);
	if (!ArrayMap_contains(&g_ui.state.menu, label)) {
		UIMenuState initial_state = { 0 };
		ArrayMap_insert(&g_ui.state.menu, label, initial_state);
	}
	UIMenuState* menu_state = UI_menu_state(menu->label);

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

	menu_state->element_closed = false;

	/* Handle focus */
	if (g_ui.input.down_pressed) {
		menu_state->focused_item++;
	}

	if (g_ui.input.up_pressed) {
		menu_state->focused_item--;
	}
}

void UI_menu_end(void) {
	UIMenu* menu = UI_current_menu();
	UIMenuState* menu_state = UI_menu_state(menu->label);
	DEBUG_ASSERT_IS_WITHIN_UI_FRAME();
	DEBUG_ASSERT(menu != NULL, "UI_menu_end() called without corresponding UI_menu_begin()");
	DEBUG_ASSERT(!menu_state->element_closed, "UI_menu_end() called without corresponding UI_menu_begin()");
	if (menu->num_items > 0) {
		menu_state->focused_item = (menu->num_items + menu_state->focused_item) % menu->num_items;
	}
	menu_state->element_closed = true;
}

bool UI_menu_item(const char* label) {
	UIMenu* menu = UI_current_menu();
	UIMenuState* menu_state = UI_menu_state(menu->label);
	DEBUG_ASSERT(menu != NULL, "UI_menu_item() called without UI_menu_begin()");
	DEBUG_ASSERT(!menu_state->element_closed, "UI_menu_item() called without UI_menu_begin()");

	/* Add item */
	const int item_index = menu->num_items++;
	UIMenuItem* item = &menu->items[item_index];
	strncpy_s(item->label, UIMenu_MaxLabelLength, label, _TRUNCATE);

	item->is_focused = menu_state->focused_item == item_index;

	/* Handle selection */
	const bool is_selected = g_ui.input.select_pressed && item->is_focused;
	return is_selected;
}
