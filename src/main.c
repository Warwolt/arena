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
			ClearBackground(BLACK);

			// Draw sprite sheet
			const Vector2 sprite_size = { 64, 64 };
			Rectangle sprite_rect = { 0, 0, 64, 64 };
			Vector2 sprite_pos = { (GetScreenWidth() - sprite_size.x) / 2, (GetScreenHeight() - sprite_size.y) / 2 };
			DrawTextureRec(spinning_donut_texture, sprite_rect, sprite_pos, WHITE);
		}
		EndDrawing();
	}

exit_program:
	CloseWindow();
	return 0;
}
