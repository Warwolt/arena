#include "platform/assert.h"

#include <windows.h>

void debug_exit(const char* message) {
	MessageBoxA(NULL, message, "Fatal Error", MB_OK | MB_ICONERROR);
	DebugBreak();
	exit(1);
}
