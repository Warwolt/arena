#include "component.h"
#include "entity.h"
#include "logging.h"
#include "resource.h"
#include "sparse_array.h"
#include "win32.h"

#include "raylib_extra.h"
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

int compare_position_ids_by_y_coordinate(void* ctx, const void* lhs, const void* rhs) {
	EntityManager* entities = (EntityManager*)ctx;
	EntityID lhs_id = *(const EntityID*)lhs;
	EntityID rhs_id = *(const EntityID*)rhs;
	Vector2 lhs_pos = { 0 };
	Vector2 rhs_pos = { 0 };
	EntityManager_get_position(entities, lhs_id, &lhs_pos);
	EntityManager_get_position(entities, rhs_id, &rhs_pos);
	if (lhs_pos.y < rhs_pos.y) {
		return -1;
	} else if (lhs_pos.y == rhs_pos.y) {
		return 0;
	} else {
		return 1;
	}
}

EntityID add_physical_object(EntityManager* entities, Vector2 position, Sprite sprite, Shape collision_shape) {
	EntityID id = EntityManager_add_entity(entities, position);
	EntityManager_add_sprite(entities, id, sprite);
	EntityManager_add_collision_shape(entities, id, collision_shape);
	return id;
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

	/* State */

	/* Run program */
	while (!WindowShouldClose()) {
		/* Update */
		{
		}

		/* Render scene */
		BeginTextureMode(screen_texture);
		{
			ClearBackground(PURPLE);
			const int font_size = 64;
			const char* text = "Scene One";
			int text_width = MeasureText(text, font_size);
			DrawText(text, (RESOLUTION_WIDTH - text_width) / 2, (RESOLUTION_HEIGHT - font_size) / 2, font_size, WHITE);
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
	ResourceManager_unload_resources(&resources);
	CloseWindow();
	return 0;
}
