#pragma once

#include "platform/window.h"

#include <raylib.h>

typedef struct Mouse {
	int x;
	int y;
} Mouse;

typedef struct Input {
	Mouse mouse; // relative to viewport
} Input;

void Input_update(Input* input, const Window* window);
