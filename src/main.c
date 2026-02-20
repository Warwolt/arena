#include <raylib.h>

int main(void) {
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
