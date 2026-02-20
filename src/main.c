#include <raylib.h>
#include <raymath.h>

#include "logging.h"

#include <math.h>
#include <stdio.h>

typedef struct Spritesheet {
	Texture2D texture;
	int sprite_width;
	int sprite_height;
} Spritesheet;

Texture2D load_texture_from_file(const char* filename) {
	Image image = LoadImage(filename);
	if (image.data == NULL) {
		return (Texture2D) { 0 };
	}
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	return texture;
}

void draw_sprite_centered(const Spritesheet* spritesheet, int sprite_index, Vector2 position, Color tint) {
	Rectangle rect = {
		.x = spritesheet->sprite_width * sprite_index,
		.y = spritesheet->sprite_height * sprite_index,
		.width = spritesheet->sprite_width,
		.height = spritesheet->sprite_height,
	};
	Vector2 centered_position = {
		.x = position.x - rect.width / 2,
		.y = position.y - rect.height / 2,
	};
	DrawTextureRec(spritesheet->texture, rect, centered_position, tint);
}

int main(void) {
	/* Init */
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 450, "Program");
	LOG_INFO("Created window");

	/* Load resources */
	Spritesheet donut_spritesheet = {
		.texture = load_texture_from_file("resource/image/spinning_donut.png"),
		.sprite_width = 64,
		.sprite_height = 64,
	};
	Spritesheet coffee_spritesheet = {
		.texture = load_texture_from_file("resource/image/spinning_coffee.png"),
		.sprite_width = 64,
		.sprite_height = 64,
	};

	/* Run program */
	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(LIME);

			const int time_now = GetTime() * 1000; // ms
			const int frame_time = 70; // ms
			const int num_frames = 12;
			const int index = (time_now % (num_frames * frame_time)) / frame_time;

			// Draw donut
			{
				Vector2 position = {
					GetScreenWidth() / 2 - 48,
					GetScreenHeight() / 2,
				};
				draw_sprite_centered(&donut_spritesheet, index, position, WHITE);
			}

			// Draw coffee
			{
				Vector2 position = {
					GetScreenWidth() / 2 + 48,
					GetScreenHeight() / 2,
				};
				draw_sprite_centered(&coffee_spritesheet, index, position, WHITE);
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

	/* Shutdown */
	CloseWindow();
	return 0;
}
