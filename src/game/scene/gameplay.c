#include "game/scene/gameplay.h"

#include "core/for_each.h"
#include "game/game.h"
#include "platform/logging.h"

#include <raylib.h>
#include <raymath.h>

#include <stdlib.h>
#include <string.h>

static EntityID add_physical_object(EntityManager* entities, Vector2 position, Sprite sprite, Shape collision_shape) {
	EntityID id = EntityManager_add_entity(entities, position);
	EntityManager_add_sprite(entities, id, sprite);
	EntityManager_add_collision_shape(entities, id, collision_shape);
	return id;
}

static int compare_position_ids_by_y_coordinate(EntityManager* entities, const EntityID* lhs_id, const EntityID* rhs_id) {
	Vector2 lhs_pos = { 0 };
	Vector2 rhs_pos = { 0 };
	EntityManager_get_position(entities, *lhs_id, &lhs_pos);
	EntityManager_get_position(entities, *rhs_id, &rhs_pos);
	if (lhs_pos.y < rhs_pos.y) {
		return -1;
	} else if (lhs_pos.y == rhs_pos.y) {
		return 0;
	} else {
		return 1;
	}
}

void Gameplay_initialize(Game* game) {
	const int screen_width = game->screen.texture.width;
	const int screen_height = game->screen.texture.height;

	const Sprite player_sprite = {
		.texture_id = ResourceManager_load_texture(&game->resources, "resource/image/pill.png"),
		.clip_rect = (Rectangle) { 0, 0, 64, 64 },
	};
	const Sprite donut_sprite = {
		.texture_id = ResourceManager_load_texture(&game->resources, "resource/image/spinning_donut.png"),
		.clip_rect = (Rectangle) { 0, 0, 64, 64 },
	};
	const Sprite coffee_sprite = {
		.texture_id = ResourceManager_load_texture(&game->resources, "resource/image/spinning_coffee.png"),
		.clip_rect = (Rectangle) { 0, 0, 64, 64 },
	};

	game->scene.gameplay = (Gameplay) {
		.room_width = screen_width * 2,
		.room_height = screen_height * 2,
		.camera =
			(Camera2D) {
				.target = Vector2Zero(),
				.offset = (Vector2) { screen_width / 2, screen_height / 2 },
				.rotation = 0.0f,
				.zoom = 1.0f,
			},
		.bg_texture_id = ResourceManager_load_texture(&game->resources, "resource/image/grass_tile.png"),
		.player_id = add_physical_object(&game->entities, Vector2Zero(), player_sprite, Shape_circle((Circle) { .radius = 16 })),
		.donut_id = add_physical_object(&game->entities, (Vector2) { -48, 0 }, donut_sprite, Shape_circle((Circle) { .radius = 8 })),
		.donut2_id = add_physical_object(&game->entities, (Vector2) { -112, 0 }, donut_sprite, Shape_circle((Circle) { .radius = 8 })),
		.coffe_id = add_physical_object(&game->entities, (Vector2) { 48, 0 }, coffee_sprite, Shape_circle((Circle) { .radius = 8 })),
	};
}

void Gameplay_update(Game* game) {
	Gameplay* gameplay = &game->scene.gameplay;
	const int room_width = gameplay->room_width;
	const int room_height = gameplay->room_height;
	const Vector2 room_top_left = (Vector2) {
		.x = -gameplay->room_width / 2,
		.y = -gameplay->room_height / 2,
	};
	const Vector2 room_bottom_right = (Vector2) {
		.x = room_width / 2,
		.y = room_height / 2,
	};

	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_switch_scene(game, SceneID_MainMenu);
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
		EntityManager_get_position(&game->entities, gameplay->player_id, &player_pos);

		const Vector2 delta = Vector2Scale(input_vec, delta_time * player_speed);
		const Vector2 moved_player_pos = Vector2Clamp(Vector2Add(player_pos, delta), room_top_left, room_bottom_right);
		EntityManager_set_position(&game->entities, gameplay->player_id, moved_player_pos);
	}

	/* Move camera */
	{
		Vector2 player_position = { 0 };
		EntityManager_get_position(&game->entities, gameplay->player_id, &player_position);
		const Vector2 camera_top_left_bound = {
			.x = room_top_left.x + game->screen.texture.width / 2,
			.y = room_top_left.y + game->screen.texture.height / 2,
		};
		const Vector2 camera_bottom_right_bound = {
			.x = room_bottom_right.x - game->screen.texture.width / 2,
			.y = room_bottom_right.y - game->screen.texture.height / 2,

		};
		gameplay->camera.target = Vector2Clamp(player_position, camera_top_left_bound, camera_bottom_right_bound);
	}

	/* Animate donut and coffee */
	{
		const int time_now = GetTime() * 1000; // ms
		const int frame_time = 70; // ms
		const int num_frames = 12;
		const int sprite_index = (time_now % (num_frames * frame_time)) / frame_time;

		EntityID ids[3] = {
			gameplay->donut_id,
			gameplay->donut2_id,
			gameplay->coffe_id,
		};
		for (size_t i = 0; i < sizeof(ids) / sizeof(*ids); i++) {
			EntityID id = ids[i];
			Sprite sprite = { 0 };
			EntityManager_get_sprite(&game->entities, id, &sprite);
			sprite.clip_rect.x = sprite_index * sprite.clip_rect.width;
			EntityManager_set_sprite(&game->entities, id, sprite);
		}
	}

	/* Check if player is colliding with other entities */
	{
		Vector2 player_position = { 0 };
		Shape player_collision_shape = { 0 };
		EntityManager_get_position(&game->entities, gameplay->player_id, &player_position);
		EntityManager_get_collision_shape(&game->entities, gameplay->player_id, &player_collision_shape);
		for (size_t i = 0; i < game->entities.components.collision_shapes.size; i++) {
			EntityID id = { game->entities.components.collision_shapes.keys[i] };
			if (id.value == gameplay->player_id.value) {
				continue;
			}
			Vector2 other_position = { 0 };
			Shape other_collision_shape = { 0 };
			EntityManager_get_position(&game->entities, id, &other_position);
			EntityManager_get_collision_shape(&game->entities, id, &other_collision_shape);
			if (player_collision_shape.type == ShapeType_Circle && other_collision_shape.type == ShapeType_Circle) {
				Circle player_circle = {
					.center = Vector2Add(player_collision_shape.circle.center, player_position),
					.radius = player_collision_shape.circle.radius,
				};
				Circle other_circle = {
					.center = Vector2Add(other_collision_shape.circle.center, other_position),
					.radius = other_collision_shape.circle.radius,
				};
				const bool is_colliding =
					CheckCollisionCircles(player_circle.center, player_circle.radius, other_circle.center, other_circle.radius);
				if (is_colliding) {
					EntityManager_remove_entity(&game->entities, id);
				}
			}
		}
	}
}

void Gameplay_render(const Game* game) {
	const Gameplay* gameplay = &game->scene.gameplay;
	const Vector2 room_top_left = (Vector2) {
		.x = -gameplay->room_width / 2,
		.y = -gameplay->room_height / 2,
	};

	/* Render in camera */
	BeginMode2D(gameplay->camera);
	{
		/* Draw background */
		ClearBackground(BLACK);
		Texture2D bg_texture = { 0 };
		ResourceManager_get_texture(&game->resources, gameplay->bg_texture_id, &bg_texture);
		DrawTextureRec(bg_texture, (Rectangle) { .width = gameplay->room_width, .height = gameplay->room_height }, room_top_left, WHITE);

		// y-sort the entities
		EntityID y_sorted_entities[MAX_NUM_ENTITES] = { 0 };
		size_t num_y_sorted_entities = game->entities.entities.size;
		memcpy(y_sorted_entities, game->entities.entities.keys, MAX_NUM_ENTITES * sizeof(EntityID));
		qsort_s(
			y_sorted_entities,
			num_y_sorted_entities,
			sizeof(EntityID),
			compare_position_ids_by_y_coordinate,
			(EntityManager*)&game->entities
		);

		/* Draw sprites */
		for_each(entity_id, y_sorted_entities, num_y_sorted_entities) {
			Vector2 position = { 0 };
			Sprite sprite = { 0 };
			Texture texture = { 0 };
			EntityManager_get_position(&game->entities, *entity_id, &position);
			EntityManager_get_sprite(&game->entities, *entity_id, &sprite);
			ResourceManager_get_texture(&game->resources, sprite.texture_id, &texture);
			Vector2 sprite_top_left = (Vector2) {
				.x = position.x - sprite.clip_rect.width / 2,
				.y = position.y - sprite.clip_rect.height / 2,
			};

			/* Draw current sprite */
			DrawTextureRec(texture, sprite.clip_rect, sprite_top_left, WHITE);
		}

		/* Draw collision shapes */
		if (game->show_debug_overlay) {
			for_each(entity, game->entities.entities.values, game->entities.entities.size) {
				if (EntityManager_has_component(&game->entities, entity->id, ComponentType_CollisionShape)) {
					Vector2 position = { 0 };
					Shape collision_shape = { 0 };
					EntityManager_get_position(&game->entities, entity->id, &position);
					EntityManager_get_collision_shape(&game->entities, entity->id, &collision_shape);
					switch (collision_shape.type) {
						case ShapeType_Circle:
							DrawCircle(position.x, position.y, collision_shape.circle.radius, ColorAlpha(GREEN, 0.5f));
							DrawCircleLines(position.x, position.y, collision_shape.circle.radius, GREEN);
							break;
					}
				}
			}
		}
	}
	EndMode2D();
}
