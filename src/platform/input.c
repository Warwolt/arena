#include "platform/input.h"

const char* InputAction_to_string(InputAction action) {
	switch (action) {
		case InputAction_Up:
			return "up";
		case InputAction_Down:
			return "down";
		case InputAction_Left:
			return "left";
		case InputAction_Right:
			return "right";
		case InputAction_Select:
			return "select";
		case InputAction_Back:
			return "back";
		case InputAction_Count:
			return "count";
	}
	return "";
}

void Input_update(Input* input, const Window* window) {
	Vector2 global_mouse = Raylib_GetMousePosition();
	input->mouse.x = (global_mouse.x - window->letterbox.x) / window->scale;
	input->mouse.y = (global_mouse.y - window->letterbox.y) / window->scale;

	int key_bindings[InputAction_Count][2] = {
		[InputAction_Up] = {'W', KEY_UP},
		[InputAction_Down] = {'S', KEY_DOWN},
		[InputAction_Left] = {'A', KEY_LEFT},
		[InputAction_Right] = {'D', KEY_RIGHT},
		[InputAction_Select] = {KEY_ENTER},
		[InputAction_Back] = {KEY_ESCAPE},
	};
	for (int action = 0; action < InputAction_Count; action++) {
		bool any_key_pressed = false;
		any_key_pressed |= Raylib_IsKeyPressed(key_bindings[action][0]);
		any_key_pressed |= Raylib_IsKeyPressed(key_bindings[action][1]);
		input->action_is_pressed[action] = any_key_pressed;
	}
}
