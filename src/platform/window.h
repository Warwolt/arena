#pragma once

#include "platform/lean_mean_windows.h"

typedef struct Window {
	WINDOWPLACEMENT placement;
} Window;

void Window_toggle_fullscreen(Window* window);
