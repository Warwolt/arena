#include "component.h"
#include "entity.h"
#include "logging.h"
#include "map.h"
#include "resource.h"
#include "win32.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UFO 50 is 16:9 at 384x216 resolution
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

void DrawTextureRecSheared(Texture2D texture, Rectangle source, Vector2 position, float angle, Color tint) {
	if (texture.id <= 0) {
		return;
	}

	Rectangle dest = { position.x, position.y, source.width, source.height };
	float width = (float)texture.width;
	float height = (float)texture.height;

	bool flipX = false;

	if (source.width < 0) {
		flipX = true;
		source.width *= -1;
	}
	if (source.height < 0)
		source.y -= source.height;

	if (dest.width < 0)
		dest.width *= -1;
	if (dest.height < 0)
		dest.height *= -1;

	float tanAngle = tanf(DEG2RAD * angle);

	// coordinates relative to top left of quad
	Vector2 localTopLeft = { 0.0f, 0.0f };
	Vector2 localTopRight = { dest.width, 0.0f };
	Vector2 localBottomLeft = { 0.0f, dest.height };
	Vector2 localBottomRight = { dest.width, dest.height };

	// world space coordinates of sheared quad
	Vector2 shearedTopLeft = {
		position.x + localTopLeft.x + tanAngle * localTopLeft.y,
		position.y + localTopLeft.y,
	};
	Vector2 shearedTopRight = {
		position.x + localTopRight.x + tanAngle * localTopRight.y,
		position.y + localTopRight.y,
	};
	Vector2 shearedBottomLeft = {
		position.x + localBottomLeft.x + tanAngle * localBottomLeft.y,
		position.y + localBottomLeft.y,
	};
	Vector2 shearedBottomRight = {
		position.x + localBottomRight.x + tanAngle * localBottomRight.y,
		position.y + localBottomRight.y,
	};

	rlSetTexture(texture.id);
	rlBegin(RL_QUADS);

	rlColor4ub(tint.r, tint.g, tint.b, tint.a);
	rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

	// Top-left corner for texture and quad
	if (flipX)
		rlTexCoord2f((source.x + source.width) / width, source.y / height);
	else
		rlTexCoord2f(source.x / width, source.y / height);
	rlVertex2f(shearedTopLeft.x, shearedTopLeft.y);

	// Bottom-left corner for texture and quad
	if (flipX)
		rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
	else
		rlTexCoord2f(source.x / width, (source.y + source.height) / height);
	rlVertex2f(shearedBottomLeft.x, shearedBottomLeft.y);

	// Bottom-right corner for texture and quad
	if (flipX)
		rlTexCoord2f(source.x / width, (source.y + source.height) / height);
	else
		rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
	rlVertex2f(shearedBottomRight.x, shearedBottomRight.y);

	// Top-right corner for texture and quad
	if (flipX)
		rlTexCoord2f(source.x / width, source.y / height);
	else
		rlTexCoord2f((source.x + source.width) / width, source.y / height);
	rlVertex2f(shearedTopRight.x, shearedTopRight.y);

	rlEnd();
	rlSetTexture(0);
}

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
	TextureID player_texture_id = ResourceManager_load_texture(&resources, "resource/image/pill.png");
	TextureID donut_texture_id = ResourceManager_load_texture(&resources, "resource/image/spinning_donut.png");
	TextureID coffee_texture_id = ResourceManager_load_texture(&resources, "resource/image/spinning_coffee.png");
	TextureID room_texture_id = ResourceManager_load_texture(&resources, "resource/image/grass_tile.png");

	/* State */
	const int room_width = RESOLUTION_WIDTH * 2;
	const int room_height = RESOLUTION_HEIGHT * 2;
	const Vector2 room_top_left = (Vector2) {
		.x = -room_width / 2,
		.y = -room_height / 2,
	};
	const Vector2 room_bottom_right = (Vector2) {
		.x = room_width / 2,
		.y = room_height / 2,
	};
	Camera2D camera = {
		.target = Vector2Zero(),
		.offset = { RESOLUTION_WIDTH / 2, RESOLUTION_HEIGHT / 2 },
		.rotation = 0.0f,
		.zoom = 1.0f,
	};
	bool show_debug_overlay = false;
	bool show_collision_shapes = false;
	EntityManager entities = { 0 };

	EntityID player_id = add_physical_object(
		&entities,
		Vector2Zero(),
		(Sprite) {
			.texture_id = player_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		},
		Shape_circle((Circle) { .radius = 16 })
	);

	// EntityID donut_id = { 0 };
	// EntityID donut_id2 = { 0 };
	// EntityID coffee_id = { 0 };

	EntityID donut_id = add_physical_object(
		&entities,
		(Vector2) { -48, 0 },
		(Sprite) {
			.texture_id = donut_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		},
		Shape_circle((Circle) { .radius = 8 })
	);

	EntityID donut_id2 = add_physical_object(
		&entities,
		(Vector2) { -112, 0 },
		(Sprite) {
			.texture_id = donut_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		},
		Shape_circle((Circle) { .radius = 8 })
	);

	EntityID coffee_id = add_physical_object(
		&entities,
		(Vector2) { 48, 0 },
		(Sprite) {
			.texture_id = coffee_texture_id,
			.clip_rect = { 0, 0, 64, 64 },
		},
		Shape_circle((Circle) { .radius = 8 })
	);

	/* Run program */
	while (!WindowShouldClose()) {
		const int time_now = GetTime() * 1000; // ms

		/* Update */
		{
			/* Function keys */
			{
				if (IsKeyPressed(KEY_F11)) {
					toggle_fullscreen();
				}

				if (IsKeyPressed(KEY_F3)) {
					show_debug_overlay = !show_debug_overlay;
				}
			}

			/* Move player */
			{
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

				Vector2 player_pos = Vector2Zero();
				const float delta_time = GetFrameTime();
				const float player_speed = 300; // px / second
				EntityManager_get_position(&entities, player_id, &player_pos);

				const Vector2 delta = Vector2Scale(input_vec, delta_time * player_speed);
				const Vector2 moved_player_pos = Vector2Clamp(Vector2Add(player_pos, delta), room_top_left, room_bottom_right);
				EntityManager_set_position(&entities, player_id, moved_player_pos);
			}

			/* Animate donut and coffee */
			{
				const int frame_time = 70; // ms
				const int num_frames = 12;
				const int sprite_index = (time_now % (num_frames * frame_time)) / frame_time;

				EntityID ids[3] = {
					donut_id,
					donut_id2,
					coffee_id,
				};
				for (size_t i = 0; i < sizeof(ids) / sizeof(*ids); i++) {
					EntityID id = ids[i];
					Sprite sprite = { 0 };
					EntityManager_get_sprite(&entities, id, &sprite);
					sprite.clip_rect.x = sprite_index * sprite.clip_rect.width;
					EntityManager_set_sprite(&entities, id, sprite);
				}
			}

			/* Check if player is colliding with other entities */
			{
				Vector2 player_position = { 0 };
				Shape player_collision_shape = { 0 };
				EntityManager_get_position(&entities, player_id, &player_position);
				EntityManager_get_collision_shape(&entities, player_id, &player_collision_shape);
				for (size_t i = 0; i < entities.components.collision_shapes.size; i++) {
					EntityID id = { entities.components.collision_shapes.keys[i] };
					if (id.value == player_id.value) {
						continue;
					}
					Vector2 other_position = { 0 };
					Shape other_collision_shape = { 0 };
					EntityManager_get_position(&entities, id, &other_position);
					EntityManager_get_collision_shape(&entities, id, &other_collision_shape);
					if (player_collision_shape.type == ShapeType_Circle && other_collision_shape.type == ShapeType_Circle) {
						Circle player_circle = {
							.center = Vector2Add(player_collision_shape.circle.center, player_position),
							.radius = player_collision_shape.circle.radius,
						};
						Circle other_circle = {
							.center = Vector2Add(other_collision_shape.circle.center, other_position),
							.radius = other_collision_shape.circle.radius,
						};
						const bool is_colliding = CheckCollisionCircles(
							player_circle.center,
							player_circle.radius,
							other_circle.center,
							other_circle.radius
						);
						if (is_colliding) {
							EntityManager_remove_entity(&entities, id);
						}
					}
				}
			}

			/* Move camera */
			{
				Vector2 player_position = { 0 };
				EntityManager_get_position(&entities, player_id, &player_position);
				const Vector2 camera_top_left_bound = {
					.x = room_top_left.x + RESOLUTION_WIDTH / 2,
					.y = room_top_left.y + RESOLUTION_HEIGHT / 2,
				};
				const Vector2 camera_bottom_right_bound = {
					.x = room_bottom_right.x - RESOLUTION_WIDTH / 2,
					.y = room_bottom_right.y - RESOLUTION_HEIGHT / 2,

				};
				camera.target = Vector2Clamp(player_position, camera_top_left_bound, camera_bottom_right_bound);
			}
		}

		/* Render scene */
		BeginTextureMode(screen_texture);
		{
			/* Render in camera */
			BeginMode2D(camera);
			{
				// Draw background
				ClearBackground(BLACK);
				Texture2D room_texture = { 0 };
				ResourceManager_get_texture(&resources, room_texture_id, &room_texture);
				DrawTextureRec(room_texture, (Rectangle) { .width = room_width, .height = room_height }, room_top_left, WHITE);

				/* Compute y-sorted entity list */
				EntityID y_sorted_entities[MAX_NUM_ENTITES] = { 0 };
				{
					memcpy(y_sorted_entities, entities.entities.keys, MAX_NUM_ENTITES * sizeof(EntityID));
					qsort_s(y_sorted_entities, entities.entities.size, sizeof(EntityID), compare_position_ids_by_y_coordinate, &entities);
				}

				/* Render shadows */
				for (int i = 0; i < entities.entities.size; i++) {
					EntityID id = y_sorted_entities[i];
					Vector2 position = { 0 };
					Sprite sprite = { 0 };
					Texture texture = { 0 };
					EntityManager_get_position(&entities, id, &position);
					EntityManager_get_sprite(&entities, id, &sprite);
					ResourceManager_get_texture(&resources, sprite.texture_id, &texture);

					Vector2 top_left = (Vector2) {
						.x = position.x - sprite.clip_rect.width - 2,
						.y = position.y - sprite.clip_rect.height / 2 - 2,
					};
					DrawTextureRecSheared(texture, sprite.clip_rect, top_left, 30.0f, ColorAlpha(BLACK, 0.17f));
				}

				/* Render sprites */
				for (int i = 0; i < entities.entities.size; i++) {
					EntityID id = y_sorted_entities[i];
					Vector2 position = { 0 };
					Sprite sprite = { 0 };
					Texture texture = { 0 };
					EntityManager_get_position(&entities, id, &position);
					EntityManager_get_sprite(&entities, id, &sprite);
					ResourceManager_get_texture(&resources, sprite.texture_id, &texture);

					Vector2 top_left = (Vector2) {
						.x = position.x - sprite.clip_rect.width / 2,
						.y = position.y - sprite.clip_rect.height / 2,
					};

					/* Render sprite*/
					DrawTextureRec(texture, sprite.clip_rect, top_left, WHITE); // sprite
				}

				/* Render collision shapes */
				if (show_collision_shapes) {
					for (int i = 0; i < entities.components.collision_shapes.size; i++) {
						EntityID id = entities.entities.values[i].id;
						Vector2 position = { 0 };
						Shape collision_shape = { 0 };
						EntityManager_get_position(&entities, id, &position);
						EntityManager_get_collision_shape(&entities, id, &collision_shape);

						switch (collision_shape.type) {
							case ShapeType_Circle:
								DrawCircle(position.x, position.y, collision_shape.circle.radius, ColorAlpha(GREEN, 0.5f));
								DrawCircleLines(position.x, position.y, collision_shape.circle.radius, GREEN);
								break;
						}
					}
				}
			}
			EndMode2D();

			/* Rendder HUD */
			{
				// Draw FPS
				if (show_debug_overlay) {
					int font_size = 24;
					int row = 0;
					char text[128] = { 0 };

					snprintf(text, sizeof(text), "FPS: %d", GetFPS());
					DrawText(text, 1, row++ * font_size, font_size, WHITE);

					snprintf(text, sizeof(text), "Entities: %zu", entities.entities.size);
					DrawText(text, 1, row++ * font_size, font_size, WHITE);

					Vector2 player_position = { 0 };
					EntityManager_get_position(&entities, player_id, &player_position);
					snprintf(text, sizeof(text), "Position: %2.f %2.f", player_position.x, player_position.y);
					DrawText(text, 1, row++ * font_size, font_size, WHITE);
				}
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
	ResourceManager_unload_resources(&resources);
	CloseWindow();
	return 0;
}
