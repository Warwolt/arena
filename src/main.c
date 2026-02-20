#include <raylib.h>
#include <raymath.h>

#include "logging.h"

#include <math.h>
#include <stdio.h>

Texture2D load_texture_from_file(const char* filename) {
	Image image = LoadImage(filename);
	if (image.data == NULL) {
		return (Texture2D) { 0 };
	}
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	return texture;
}

int main(void) {
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 450, "Program");
	LOG_INFO("Created window");

	Texture2D donut_texture = load_texture_from_file("resource/image/spinning_donut.png");
	Texture2D coffee_texture = load_texture_from_file("resource/image/spinning_coffee.png");

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(LIME);

			const int time_now = GetTime() * 1000; // ms
			const int frame_time = 80; // ms
			const int num_frames = 12;
			const int index = (time_now % (num_frames * frame_time)) / frame_time;
			const Vector2 sprite_size = { 64, 64 };

			// Draw donut
			{
				Rectangle sprite_rect = { 64 * index, 64 * index, 64, 64 };
				Vector2 sprite_pos = {
					(GetScreenWidth() - sprite_size.x) / 2 - 48,
					(GetScreenHeight() - sprite_size.y) / 2,
				};
				DrawTextureRec(donut_texture, sprite_rect, Vector2Add(sprite_pos, (Vector2) { -4, 8 }), (Color) { 0, 0, 0, 16 });
				DrawTextureRec(donut_texture, sprite_rect, sprite_pos, WHITE);
			}

			// Draw coffee
			{
				Rectangle sprite_rect = { 64 * index, 64 * index, 64, 64 };
				Vector2 sprite_pos = {
					(GetScreenWidth() - sprite_size.x) / 2 + 48,
					(GetScreenHeight() - sprite_size.y) / 2,
				};
				DrawTextureRec(coffee_texture, sprite_rect, Vector2Add(sprite_pos, (Vector2) { -4, 8 }), (Color) { 0, 0, 0, 16 });
				DrawTextureRec(coffee_texture, sprite_rect, sprite_pos, WHITE);
			}

			// Draw FPS
			const bool draw_debug_overlay = false;
			if (draw_debug_overlay) {
				char text[128] = { 0 };
				snprintf(text, 128, "FPS: %d", GetFPS());
				DrawText(text, 0, 0, 16, WHITE);
			}
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
