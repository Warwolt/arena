#pragma once

#include <stdbool.h>

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

	// FIXME: move this to private struct in ui.c?
	// state
	int focused_item;
	bool is_open;

} UIMenu;

typedef struct UIView {
	UIMenu menus[UIMenu_MaxMenus];
	int num_menus;
} UIView;

typedef struct UIInput {
	bool down_pressed;
	bool up_pressed;
	bool select_pressed;
} UIInput;

void UI_initialize(void);
const UIView* UI_view(void);

void UI_begin(UIInput input);
void UI_end(void);

void UI_menu_begin(const char* label);
void UI_menu_end(void);

bool UI_menu_item(const char* label);
