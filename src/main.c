#include <raylib.h>
#include <raymath.h>

#include "logging.h"

#include <math.h>

int main(void) {
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 450, "Program");
	LOG_INFO("Created window");

	// Load sprite sheet
	const char* image_path = "resource/image/spinning_donut.png";
	Image spinning_donut_image = LoadImage(image_path);
	if (spinning_donut_image.data == NULL) {
		LOG_ERROR("Couldn't load image \"%s\"!", image_path);
		goto exit_program;
	}
	Texture2D spinning_donut_texture = LoadTextureFromImage(spinning_donut_image);
	UnloadImage(spinning_donut_image);

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(LIME);

			const int time_now = GetTime() * 1000; // ms
			const int frame_time = 70; // ms
			const int num_frames = 12;
			const int index = (time_now % (num_frames * frame_time)) / frame_time;

			// Draw sprite sheet
			const Vector2 sprite_size = { 64, 64 };
			Rectangle sprite_rect = { 64 * index, 64 * index, 64, 64 };
			Vector2 sprite_pos = { (GetScreenWidth() - sprite_size.x) / 2, (GetScreenHeight() - sprite_size.y) / 2 };
			DrawTextureRec(spinning_donut_texture, sprite_rect, Vector2Add(sprite_pos, (Vector2) { -8, 12 }), (Color) { 0, 0, 0, 16 });
			DrawTextureRec(spinning_donut_texture, sprite_rect, sprite_pos, WHITE);
		}
		EndDrawing();
	}

exit_program:
	CloseWindow();
	return 0;
}
