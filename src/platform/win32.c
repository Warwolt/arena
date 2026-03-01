#include "platform/win32.h"

#include <shlwapi.h>
#include <stdio.h>
#include <windows.h>

// forward declare from raylib.h
void* GetWindowHandle(void);

void Win32_set_process_dpi_aware(void) {
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

// Based on Raymond Chen's "How do I switch a window between normal and fullscreen?"
// https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
void Win32_toggle_fullscreen() {
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

void Win32_show_error_message_box(const char* text) {
	HWND handle = GetWindowHandle();
	MessageBoxA(handle, text, "Error", MB_OK | MB_ICONERROR);
}

void Win32_get_executable_directory(char* buffer) {
	GetModuleFileName(NULL, buffer, MAX_PATH);
	PathRemoveFileSpecA(buffer);
}

bool Win32_file_exists(const char* full_path) {
	return PathFileExistsA(full_path);
}

bool Win32_copy_file(const char* src_path, const char* dst_path) {
	return CopyFileA(src_path, dst_path, false) != 0;
}
