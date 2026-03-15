#pragma once

#include "platform/lean_mean_windows.h"

#include <raylib.h>

// Represents a window at a given resolution.
// Even if the aplication window is re-sized, the viewport remains the same size unless the window
// resolution is changed. Render logic is based on the viewport, not the application window.
typedef struct Window {
	WINDOWPLACEMENT placement; // used for fullscreen toggling
	RenderTexture viewport;
	Rectangle letterbox; // used to draw the viewport to the window
} Window;

Window Window_initialize(int width, int height);
void Window_deinitialize(Window* window);
void Window_update(Window* window);
void Window_toggle_fullscreen(Window* window);

inline int Window_width(const Window* window) {
	return window->viewport.texture.width;
}

inline int Window_height(const Window* window) {
	return window->viewport.texture.height;
}

inline Rectangle Window_rectangle(const Window* window) {
	return (Rectangle) {
		.width = window->viewport.texture.width,
		.height = window->viewport.texture.height,
	};
}
