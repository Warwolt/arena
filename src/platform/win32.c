#include "platform/win32.h"

#include <shlwapi.h>
#include <stdio.h>
#include <windows.h>

// forward declare from raylib.h
void* GetWindowHandle(void);

static DWORD WINAPI run_command_thread(LPVOID* arg) {
	const char* command = (char*)arg;

	/* Setup pipes for stdout and stderr */
	HANDLE stdout_read = NULL;
	HANDLE stdout_write = NULL;
	SECURITY_ATTRIBUTES security_attributes = {
		.nLength = sizeof(SECURITY_ATTRIBUTES),
		.lpSecurityDescriptor = NULL,
		.bInheritHandle = TRUE,
	};
	BOOL pipe_was_created = CreatePipe(&stdout_read, &stdout_write, &security_attributes, 0);
	if (!pipe_was_created) {
		fprintf(stderr, "%s:%d CreatePipe failed\n", __FILE__, __LINE__);
		return 1;
	}

	/* Ensure the read handle to the pipe is not inherited. */
	SetHandleInformation(stdout_read, HANDLE_FLAG_INHERIT, 0);

	/* Create process */
	PROCESS_INFORMATION process_info = { 0 };
	STARTUPINFOA startup_info = {
		.cb = sizeof(STARTUPINFOA),
		.dwFlags = STARTF_USESTDHANDLES,
		.hStdInput = NULL,
		.hStdOutput = stdout_write,
		.hStdError = stdout_write,
	};
	BOOL process_was_created = CreateProcessA(
		NULL, // lpApplicationName
		(LPSTR)(command), // lpCommandLine
		NULL, // lpProcessAttributes
		NULL, // lpThreadAttributes
		TRUE, // bInheritHandles
		0, // dwCreationFlags
		NULL, // lpEnvironment
		NULL, // lpCurrentDirectory
		&startup_info, // lpStartupInfo
		&process_info // lpProcessInformation
	);
	if (!process_was_created) {
		fprintf(stderr, "%s:%d CreateProcess failed: %d\n", __FILE__, __LINE__, GetLastError());
	}

	/* Close write end of the pipe in the parent process */
	CloseHandle(stdout_write);

	/* Print stdout of command */
	while (1) {
		char buffer[4096] = "";
		DWORD num_read_bytes = 0;
		BOOL success = ReadFile(stdout_read, buffer, sizeof(buffer) - 1, &num_read_bytes, NULL);
		if (!success || num_read_bytes == 0) {
			break;
		}

		buffer[num_read_bytes] = '\0';
		printf("%s", buffer);
		fflush(stdout);
	}

	/* Wait for process to finish */
	WaitForSingleObject(process_info.hProcess, INFINITE);

	/* Free string */
	free((void*)command);

	return 0;
}

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

void Win32_run_command(const char* command) {
	const char* copied_command = strdup(command);
	CreateThread(NULL, 0, run_command_thread, (LPVOID)copied_command, 0, NULL);
}
