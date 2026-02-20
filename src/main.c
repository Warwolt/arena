#include <raylib.h>
#include <raymath.h>

#include "logging.h"
#include "win32.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

void draw_sprite_centered(Spritesheet spritesheet, int sprite_index, Vector2 position, Color tint) {
	Rectangle rect = {
		.x = spritesheet.sprite_width * sprite_index,
		.y = spritesheet.sprite_height * sprite_index,
		.width = spritesheet.sprite_width,
		.height = spritesheet.sprite_height,
	};
	Vector2 centered_position = {
		.x = position.x - rect.width / 2,
		.y = position.y - rect.height / 2,
	};
	DrawTextureRec(spritesheet.texture, rect, centered_position, tint);
}

void draw_texture_centered(Texture2D texture, Vector2 position, Color tint) {
	Vector2 centered_position = {
		.x = position.x - texture.width / 2,
		.y = position.y - texture.height / 2,
	};
	DrawTexture(texture, centered_position.x, centered_position.y, tint);
}

// UFO 50 is 16:9 at 384x216 resolution
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

int main(void) {
	/* Init */
	initialize_logging();
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "Program");
	LOG_INFO("Created window");

	// Render texture
	RenderTexture2D screen_texture = LoadRenderTexture(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

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
	Texture2D pill_texture = load_texture_from_file("resource/image/pill.png");

	/* State */
	Vector2 player_position = { 0, 0 };

	/* Run program */
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_F11)) {
			toggle_fullscreen();
		}

		const float delta_time = GetFrameTime();
		const float speed = 150;
		if (IsKeyDown('A')) {
			player_position.x -= speed * delta_time;
		}
		if (IsKeyDown('D')) {
			player_position.x += speed * delta_time;
		}
		if (IsKeyDown('W')) {
			player_position.y -= speed * delta_time;
		}
		if (IsKeyDown('S')) {
			player_position.y += speed * delta_time;
		}

		BeginTextureMode(screen_texture);
		{
			// Draw background
			ClearBackground(LIME);

			const int time_now = GetTime() * 1000; // ms
			const int frame_time = 70; // ms
			const int num_frames = 12;
			const int index = (time_now % (num_frames * frame_time)) / frame_time;
			const Vector2 screen_middle = {
				.x = RESOLUTION_WIDTH / 2,
				.y = RESOLUTION_HEIGHT / 2,
			};

			// Draw donut
			if (0) {
				Vector2 position = { screen_middle.x - 48, screen_middle.y };
				draw_sprite_centered(donut_spritesheet, index, position, WHITE);
			}

			// Draw coffee
			if (0) {
				Vector2 position = { screen_middle.x + 48, screen_middle.y };
				draw_sprite_centered(coffee_spritesheet, index, position, WHITE);
			}

			// Draw pill
			{
				Vector2 position = Vector2Add(player_position, screen_middle);
				draw_texture_centered(pill_texture, position, WHITE);
			}

			// Draw FPS
			const bool draw_debug_overlay = false;
			if (draw_debug_overlay) {
				char text[128] = { 0 };
				snprintf(text, 128, "FPS: %d", GetFPS());
				DrawText(text, 0, 0, 16, WHITE);
			}
		}
		EndTextureMode();

		BeginDrawing();
		{
			// Draw background
			ClearBackground(BLACK);

			// Draw stretched screen texture
			int screen_width = GetScreenWidth();
			int screen_height = GetScreenHeight();
			int scale = min(screen_width / RESOLUTION_WIDTH, screen_height / RESOLUTION_HEIGHT);
			int scaled_width = scale * screen_texture.texture.width;
			int scaled_height = scale * screen_texture.texture.height;
			Rectangle scaled_screen_rect = {
				.x = (screen_width - scaled_width) / 2,
				.y = (screen_height - scaled_height) / 2,
				.width = scaled_width,
				.height = scaled_height,
			};
			DrawTexturePro(
				screen_texture.texture,
				(Rectangle) { 0, 0, screen_texture.texture.width, -screen_texture.texture.height },
				scaled_screen_rect,
				Vector2Zero(),
				0,
				WHITE
			);
		}
		EndDrawing();
	}

	/* Shutdown */
	CloseWindow();
	return 0;
}
