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

typedef struct EntityID {
	int value;
} EntityID;

// UFO 50 is 16:9 at 384x216 resolution
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

#define MAX_POSITIONS (int)128

#define INDEX_NOT_FOUND (int)-1

int find_entity_index(EntityID* ids, size_t len, EntityID id) {
	for (size_t i = 0; i < len; i++) {
		if (ids[i].value == id.value) {
			return (int)i;
		}
	}
	return -1;
}

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
	bool show_debug_overlay = false;

	EntityID position_keys[MAX_POSITIONS];
	Vector2 positions_values[MAX_POSITIONS];
	int positions_size = 0;

	// create player entity
	EntityID player_id = { 1 };

	// add position to player
	position_keys[0] = player_id;
	positions_values[0] = Vector2Zero();
	positions_size++;

	/* Run program */
	while (!WindowShouldClose()) {
		// retreive player position
		Vector2 position_value = Vector2Zero();
		{
			int position_index = find_entity_index(position_keys, MAX_POSITIONS, player_id);
			if (position_index != INDEX_NOT_FOUND) {
				position_value = positions_values[0];
			}
		}
		const Vector2 player_position = position_value;

		/* Update */
		{
			if (IsKeyPressed(KEY_F11)) {
				toggle_fullscreen();
			}

			if (IsKeyPressed(KEY_F3)) {
				show_debug_overlay = !show_debug_overlay;
			}

			Vector2 input_vector = Vector2Zero();
			if (IsKeyDown('A')) {
				input_vector = Vector2Add(input_vector, (Vector2) { -1, 0 });
			}
			if (IsKeyDown('D')) {
				input_vector = Vector2Add(input_vector, (Vector2) { 1, 0 });
			}
			if (IsKeyDown('W')) {
				input_vector = Vector2Add(input_vector, (Vector2) { 0, -1 });
			}
			if (IsKeyDown('S')) {
				input_vector = Vector2Add(input_vector, (Vector2) { 0, 1 });
			}
			input_vector = Vector2Normalize(input_vector);

			const float delta_time = GetFrameTime();
			const float player_speed = 200; // px / second
			Vector2 new_player_position = Vector2Add(player_position, Vector2Scale(input_vector, delta_time * player_speed));

			// set player position
			{
				int position_index = find_entity_index(position_keys, MAX_POSITIONS, player_id);
				if (position_index != INDEX_NOT_FOUND) {
					positions_values[0] = new_player_position;
				}
			}
		}

		/* Render scene */
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
			if (1) {
				Vector2 position = { screen_middle.x + 48, screen_middle.y };
				draw_sprite_centered(coffee_spritesheet, index, position, WHITE);
			}

			// Draw player pill
			{
				Vector2 position = Vector2Add(player_position, screen_middle);
				draw_texture_centered(pill_texture, position, WHITE);
			}

			// Draw FPS
			if (show_debug_overlay) {
				char text[128] = { 0 };
				snprintf(text, 128, "FPS: %d", GetFPS());
				DrawText(text, 0, 0, 16, WHITE);
			}
		}
		EndTextureMode();

		/* Render window */
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
