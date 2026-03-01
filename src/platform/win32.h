#pragma once

#define _AMD64_ 1
#define _M_X64 100
#include <processthreadsapi.h>
#include <stdbool.h>

// window
void Win32_set_process_dpi_aware(void);
void Win32_toggle_fullscreen(void);
void Win32_show_error_message_box(const char* text);

// file
void Win32_get_executable_directory(char* buffer);
bool Win32_file_exists(const char* full_path);
bool Win32_copy_file(const char* src_path, const char* dst_path); // true if file copied

// command
void Win32_run_command(const char* command);
