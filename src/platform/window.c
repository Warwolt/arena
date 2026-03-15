#include "platform/window.h"

#include <raylib.h>

Window Window_initialize(int width, int height) {
	return (Window) {
		.viewport = Raylib_LoadRenderTexture(width, height),
	};
}

void Window_deinitialize(Window* window) {
	Raylib_UnloadRenderTexture(window->viewport);
}

void Window_update(Window* window) {
	/* Update the paint rect for the viewport */
	int screen_width = Raylib_GetScreenWidth();
	int screen_height = Raylib_GetScreenHeight();
	int viewport_width = window->viewport.texture.width;
	int viewport_height = window->viewport.texture.height;

	int scale = min(screen_width / viewport_width, screen_height / viewport_height);
	int scaled_width = scale * viewport_width;
	int scaled_height = scale * viewport_height;
	window->letterbox = (Rectangle) {
		.x = (screen_width - scaled_width) / 2,
		.y = (screen_height - scaled_height) / 2,
		.width = scaled_width,
		.height = scaled_height,
	};
}

// Based on Raymond Chen's "How do I switch a window between normal and fullscreen?"
// https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
void Window_toggle_fullscreen(Window* window) {
	HWND handle = Raylib_GetWindowHandle();

	DWORD style = GetWindowLong(handle, GWL_STYLE);
	if (style & WS_OVERLAPPEDWINDOW) {
		GetWindowPlacement(handle, &window->placement);
		SetWindowLong(handle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO monitor_info = { .cbSize = sizeof(monitor_info) };
		GetMonitorInfo(MonitorFromWindow(handle, MONITOR_DEFAULTTOPRIMARY), &monitor_info);
		LONG monitor_x = monitor_info.rcMonitor.left;
		LONG monitor_y = monitor_info.rcMonitor.top;
		LONG monitor_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
		LONG monitor_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
		SetWindowPos(handle, HWND_TOP, monitor_x, monitor_y, monitor_width, monitor_height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	} else {
		SetWindowLong(handle, GWL_STYLE, style | WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
		SetWindowPlacement(handle, &window->placement);
		SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}
