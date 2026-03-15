#pragma once

#include "platform/window.h"

#include <raylib.h>

#include <stdbool.h>

typedef struct Mouse {
	int x;
	int y;
} Mouse;

typedef enum InputAction {
	InputAction_Up,
	InputAction_Down,
	InputAction_Left,
	InputAction_Right,
	InputAction_Select,
	InputAction_Back,
	InputAction_Count,
} InputAction;

const char* InputAction_to_string(InputAction action);

typedef struct Input {
	Mouse mouse; // relative to viewport
	bool action_is_pressed[InputAction_Count];
} Input;

void Input_update(Input* input, const Window* window);
