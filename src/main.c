#include <raylib.h>

#include "logging.h"

int main(void) {
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 450, "Program");
	LOG_INFO("Created window");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Congrats! You created your first window!", 190, 200, 20, BLACK);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
