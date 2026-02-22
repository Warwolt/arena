#include "entity.h"
#include "logging.h"
#include "memory/map.h"
#include "resource_manager.h"
#include "win32.h"

#include <raylib.h>
#include <raymath.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UFO 50 is 16:9 at 384x216 resolution
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

typedef struct Sprite {
	TextureID texture_id;
	Rectangle clip_rect;
} Sprite;

#define MAX_POSITION_COMPONENTS (int)128
#define MAX_SPRITE_COMPONENTS (int)128

typedef struct ComponentManager {
	Map(Vector2, MAX_POSITION_COMPONENTS) positions;
	Map(Sprite, MAX_SPRITE_COMPONENTS) sprites;
} ComponentManager;

void ComponentManager_add_position(ComponentManager* components, EntityID id, Vector2 position) {
	Map_insert(&components->positions, id.value, position);
}

void ComponentManager_get_position(ComponentManager* components, EntityID id, Vector2* position) {
	Map_get(&components->positions, id.value, position);
}

void ComponentManager_set_position(ComponentManager* components, EntityID id, Vector2 position) {
	Map_set(&components->positions, id.value, position);
}

void ComponentManager_add_sprite(ComponentManager* components, EntityID id, Sprite sprite) {
	Map_insert(&components->sprites, id.value, sprite);
}

void ComponentManager_get_sprite(ComponentManager* components, EntityID id, Sprite* sprite) {
	Map_get(&components->sprites, id.value, sprite);
}

void ComponentManager_set_sprite(ComponentManager* components, EntityID id, Sprite sprite) {
	Map_set(&components->sprites, id.value, sprite);
}

int compare_position_ids_by_y_coordinate(void* ctx, const void* lhs, const void* rhs) {
	ComponentManager* components = (ComponentManager*)ctx;
	EntityID lhs_id = *(const EntityID*)lhs;
	EntityID rhs_id = *(const EntityID*)rhs;
	Vector2 lhs_pos = { 0 };
	Vector2 rhs_pos = { 0 };
	ComponentManager_get_position(components, lhs_id, &lhs_pos);
	ComponentManager_get_position(components, rhs_id, &rhs_pos);
	if (lhs_pos.y < rhs_pos.y) {
		return -1;
	} else if (lhs_pos.y == rhs_pos.y) {
		return 0;
	} else {
		return 1;
	}
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
	ResourceManager resources = { 0 };
	TextureID player_texture_id = ResourceManager_load_texture(&resources, "resource/image/pill.png");
	TextureID donut_texture_id = ResourceManager_load_texture(&resources, "resource/image/spinning_donut.png");
	TextureID coffee_texture_id = ResourceManager_load_texture(&resources, "resource/image/spinning_coffee.png");

	/* State */
	bool show_debug_overlay = false;
	ComponentManager components = { 0 };
	EntityID player_id = EntityID_new();
	EntityID donut_id = EntityID_new();
	EntityID coffee_id = EntityID_new();

	// add player
	ComponentManager_add_position(&components, player_id, Vector2Zero());
	ComponentManager_add_sprite(
		&components,
		player_id,
		(Sprite) {
			.texture_id = player_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		}
	);

	// add donut
	ComponentManager_add_position(&components, donut_id, (Vector2) { -48, 0 });
	ComponentManager_add_sprite(
		&components,
		donut_id,
		(Sprite) {
			.texture_id = donut_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		}
	);

	// add coffee
	ComponentManager_add_position(&components, coffee_id, (Vector2) { 48, 0 });
	ComponentManager_add_sprite(
		&components,
		coffee_id,
		(Sprite) {
			.texture_id = coffee_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		}
	);

	/* Run program */
	while (!WindowShouldClose()) {
		// retreive player position
		Vector2 player_pos = Vector2Zero();
		ComponentManager_get_position(&components, player_id, &player_pos);

		/* Update */
		{
			if (IsKeyPressed(KEY_F11)) {
				toggle_fullscreen();
			}

			if (IsKeyPressed(KEY_F3)) {
				show_debug_overlay = !show_debug_overlay;
			}

			Vector2 input_vec = Vector2Zero();
			if (IsKeyDown('A')) {
				input_vec = Vector2Add(input_vec, (Vector2) { -1, 0 });
			}
			if (IsKeyDown('D')) {
				input_vec = Vector2Add(input_vec, (Vector2) { 1, 0 });
			}
			if (IsKeyDown('W')) {
				input_vec = Vector2Add(input_vec, (Vector2) { 0, -1 });
			}
			if (IsKeyDown('S')) {
				input_vec = Vector2Add(input_vec, (Vector2) { 0, 1 });
			}
			input_vec = Vector2Normalize(input_vec);

			const float delta_time = GetFrameTime();
			const float player_speed = 200; // px / second
			const Vector2 moved_player_pos = Vector2Add(player_pos, Vector2Scale(input_vec, delta_time * player_speed));
			ComponentManager_set_position(&components, player_id, moved_player_pos);
		}

		/* Render scene */
		BeginTextureMode(screen_texture);
		{
			const int time_now = GetTime() * 1000; // ms
			const Vector2 screen_middle = {
				.x = RESOLUTION_WIDTH / 2,
				.y = RESOLUTION_HEIGHT / 2,
			};

			// Draw background
			ClearBackground(LIME);

			// animate donut and coffee
			{
				const int frame_time = 70; // ms
				const int num_frames = 12;
				const int sprite_index = (time_now % (num_frames * frame_time)) / frame_time;

				Sprite donut_sprite = { 0 };
				ComponentManager_get_sprite(&components, donut_id, &donut_sprite);
				donut_sprite.clip_rect.x = sprite_index * donut_sprite.clip_rect.width;
				ComponentManager_set_sprite(&components, donut_id, donut_sprite);

				Sprite coffee_sprite = { 0 };
				ComponentManager_get_sprite(&components, coffee_id, &coffee_sprite);
				coffee_sprite.clip_rect.x = sprite_index * coffee_sprite.clip_rect.width;
				ComponentManager_set_sprite(&components, coffee_id, coffee_sprite);
			}

			// sort entities based on position
			EntityID y_sorted_entities[MAX_POSITION_COMPONENTS] = { 0 };
			{
				memcpy(y_sorted_entities, components.positions.keys, MAX_POSITION_COMPONENTS * sizeof(EntityID));
				qsort_s(y_sorted_entities, components.positions.size, sizeof(EntityID), compare_position_ids_by_y_coordinate, &components);
			}

			// render entities
			for (int i = 0; i < components.positions.size; i++) {
				EntityID id = y_sorted_entities[i];
				Vector2 position = { 0 };
				Sprite sprite = { 0 };
				Texture texture = { 0 };
				ComponentManager_get_position(&components, id, &position);
				ComponentManager_get_sprite(&components, id, &sprite);
				ResourceManager_get_texture(&resources, sprite.texture_id, &texture);
				Vector2 camera_space_top_left = (Vector2) {
					.x = position.x + screen_middle.x - sprite.clip_rect.width / 2,
					.y = position.y + screen_middle.y - sprite.clip_rect.height / 2,
				};
				DrawTextureRec(texture, sprite.clip_rect, camera_space_top_left, WHITE);
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
	LOG_INFO("Shutdown");
	CloseWindow();
	return 0;
}
