#include "core/debug/assert.h"

#include <stdbool.h>
#include <windows.h>

static bool g_unit_testing = false;

void enable_unit_test_debug_exit(void) {
	g_unit_testing = true;
}

void debug_exit(const char* message) {
	if (!g_unit_testing) {
		MessageBoxA(NULL, message, "Fatal Error", MB_OK | MB_ICONERROR);
		DebugBreak();
	}
	exit(1);
}
