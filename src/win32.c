#include "win32.h"

#include <windows.h>

// forward declare from raylib.h
void* GetWindowHandle(void);

// Based on Raymond Chen's "How do I switch a window between normal and fullscreen?"
// https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
void toggle_fullscreen() {
	static WINDOWPLACEMENT m_placement;
	HWND handle = GetWindowHandle();

	DWORD style = GetWindowLong(handle, GWL_STYLE);
	if (style & WS_OVERLAPPEDWINDOW) {
		GetWindowPlacement(handle, &m_placement);
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
		SetWindowPlacement(handle, &m_placement);
		SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}
