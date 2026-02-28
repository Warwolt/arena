#include "game/scene/gameplay.h"

#include "game/game.h"
#include "platform/logging.h"

#include <raylib.h>
#include <raymath.h>

void Gameplay_initialize(Game* game) {
	const int screen_width = game->screen.texture.width;
	const int screen_height = game->screen.texture.height;

	game->scene.gameplay = (Gameplay) {
		.room_width = screen_width * 2,
		.room_height = screen_height * 2,
		.camera =
			(Camera2D) {
				.target = Vector2Zero(),
				.offset = { screen_width / 2, screen_height / 2 },
				.rotation = 0.0f,
				.zoom = 1.0f,
			},
		.bg_texture_id = ResourceManager_load_texture(&game->resources, "resource/image/grass_tile.png"),
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

		// Vector2 player_pos = Vector2Zero();
		Vector2 player_pos = gameplay->player_position;
		const float delta_time = GetFrameTime();
		const float player_speed = 300; // px / second
		// EntityManager_get_position(&entities, player_id, &player_pos);

		const Vector2 delta = Vector2Scale(input_vec, delta_time * player_speed);
		const Vector2 moved_player_pos = Vector2Clamp(Vector2Add(player_pos, delta), room_top_left, room_bottom_right);
		// EntityManager_set_position(&entities, player_id, moved_player_pos);
		gameplay->player_position = moved_player_pos;
	}

	// temp
	gameplay->camera.target = gameplay->player_position;
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
		// Draw background
		ClearBackground(BLACK);
		Texture2D bg_texture = { 0 };
		ResourceManager_get_texture(&game->resources, gameplay->bg_texture_id, &bg_texture);
		DrawTextureRec(bg_texture, (Rectangle) { .width = gameplay->room_width, .height = gameplay->room_height }, room_top_left, WHITE);
	}
	EndMode2D();
}
