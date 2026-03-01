#pragma once

#include <stdbool.h>

void Win32_toggle_fullscreen(void);
void Win32_show_error_message_box(const char* text);
bool Win32_file_exists_relative_executable(const char* relative_path);
