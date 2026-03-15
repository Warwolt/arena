#include "platform/input.h"

const char* InputAction_to_string(InputAction action) {
	switch (action) {
		case InputAction_Up:
			return "Up";
		case InputAction_Down:
			return "Down";
		case InputAction_Left:
			return "Left";
		case InputAction_Right:
			return "Right";
		case InputAction_Select:
			return "Select";
		case InputAction_Back:
			return "Back";
		case InputAction_Count:
			return "Count";
		case InputAction_ToggleFullscreen:
			return "ToggleFullscreen";
		case InputAction_ShowDebugOverlay:
			return "ShowDebugOverlay";
	}
	return "";
}

void Input_update(Input* input, const Window* window) {
	/* Mouse input */
	Vector2 global_mouse = Raylib_GetMousePosition();
	input->mouse.x = (global_mouse.x - window->letterbox.x) / window->scale;
	input->mouse.y = (global_mouse.y - window->letterbox.y) / window->scale;

	/* Action input */
	// clang-format off
	int key_bindings[InputAction_Count][2] = {
		[InputAction_Up] = { 'W', KEY_UP },
		[InputAction_Down] = { 'S', KEY_DOWN },
		[InputAction_Left] = { 'A', KEY_LEFT },
		[InputAction_Right] = { 'D', KEY_RIGHT },
		[InputAction_Select] = { KEY_ENTER },
		[InputAction_Back] = { KEY_ESCAPE },
		[InputAction_ToggleFullscreen] = { KEY_F11 },
		[InputAction_ShowDebugOverlay] = { KEY_F3 },
	};
	// clang-format on

	// pressed
	for (int action = 0; action < InputAction_Count; action++) {
		bool any_key_pressed = false;
		any_key_pressed |= Raylib_IsKeyPressed(key_bindings[action][0]);
		any_key_pressed |= Raylib_IsKeyPressed(key_bindings[action][1]);
		input->action_is_pressed[action] = any_key_pressed;
	}
	// down
	for (int action = 0; action < InputAction_Count; action++) {
		bool any_key_down = false;
		any_key_down |= Raylib_IsKeyDown(key_bindings[action][0]);
		any_key_down |= Raylib_IsKeyDown(key_bindings[action][1]);
		input->action_is_down[action] = any_key_down;
	}
}
