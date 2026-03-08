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
	int focused_item;
	bool is_open;
} UIMenu;

typedef struct UIView {
	UIMenu menus[UIMenu_MaxMenus];
	int num_menus;
} UIView;

typedef struct UIState {
	bool is_within_frame;

	// FIXME:
	// This needs to be a mapping String -> Number
	// We want to use the label of the menu to identify it.
	// Can't be bothered to implement a HashMap, nor to handle the lifetimes of strings.
	// Easy solution for now is just an array [(String, Number)] and just scan through it.
	// It's an O(n) solution but we're also not going to have anything but a very small amount of menu items.
	//
	// Alternatively, do we _have_ to nuke the UIMenu state every frame?
	// What's the kind of things we need to compute, and what data do we need? How should we organize that data?
	//
	// At this point it would make sense to start writing unit test so we can better track all the
	// things that the UI is supposed to be able to do. Quite a lot of different cases to cover even
	// with this simple system.
	int focused_item;

} UIState;

typedef struct UI {
	UIView view;
	UIState state;
} UI;

UI* UI_get(void);

void UI_initialize(void);

void UI_begin(void);
void UI_end(void);

void UI_menu_begin(const char* label);
void UI_menu_end(void);

bool UI_menu_item(const char* label);
