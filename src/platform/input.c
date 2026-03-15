#include "platform/input.h"

void Input_update(Input* input, const Window* window) {
	Vector2 global_mouse = Raylib_GetMousePosition();
	input->mouse.x = (global_mouse.x - window->letterbox.x) / window->scale;
	input->mouse.y = (global_mouse.y - window->letterbox.y) / window->scale;
}
