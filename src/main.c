#include <raylib.h>

#include "logging.h"

int main(void) {
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 450, "Program");
	LOG_INFO("Created window");

	// Load sprite sheet
	const char* image_path = "resource/image/spinning_donut.png";
	Image spinning_donut_image = LoadImage(image_path);
	if (spinning_donut_image.data == NULL) {
		LOG_ERROR("Couldn't load image \"%s\"!\n", image_path);
		goto exit_program;
	}
	Texture2D spinning_donut_texture = LoadTextureFromImage(spinning_donut_image);
	UnloadImage(spinning_donut_image);

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			DrawText("Mmmmmm... donuts!", 190, 200, 20, BLACK);

			// Draw sprite sheet
			DrawTexture(spinning_donut_texture, 0, 0, WHITE);
		}
		EndDrawing();
	}

exit_program:
	CloseWindow();
	return 0;
}
