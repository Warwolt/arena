#include <raylib.h>

#include "logging.h"

int main(void) {
	initialize_logging();
	debug_log(LogLevel_Debug, __FILE__, __LINE__, "Hello World!");
	debug_log(LogLevel_Info, "main.c", __LINE__, "Hello World!");
	debug_log(LogLevel_Warning, "main.c", __LINE__, "Hello World!");
	debug_log(LogLevel_Error, "main.c", __LINE__, "Hello World!");
	debug_log(LogLevel_Fatal, "main.c", __LINE__, "Hello World!");

	SetTraceLogLevel(LOG_NONE);
	InitWindow(800, 450, "Program");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Congrats! You created your first window!", 190, 200, 20, BLACK);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
