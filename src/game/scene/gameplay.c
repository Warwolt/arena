#include "game/scene/gameplay.h"

#include "game/game.h"
#include "platform/logging.h"

#include <raylib.h>
#include <raymath.h>

void Gameplay_initialize(Game* game) {
	game->scene.gameplay = (Gameplay) {
		.bg_texture_id = ResourceManager_load_texture(&game->resources, "resource/image/grass_tile.png"),
		.room_width = game->screen.texture.width * 2,
		.room_height = game->screen.texture.height * 2,
	};
}

void Gameplay_update(Game* game) {
	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_switch_scene(game, SceneID_MainMenu);
	}
}

void Gameplay_render(const Game* game) {
	const Gameplay* gameplay = &game->scene.gameplay;
	const Vector2 room_top_left = (Vector2) {
		.x = -gameplay->room_width / 2,
		.y = -gameplay->room_height / 2,
	};

	// Draw background
	ClearBackground(BLACK);
	Texture2D bg_texture = { 0 };
	ResourceManager_get_texture(&game->resources, gameplay->bg_texture_id, &bg_texture);
	DrawTextureRec(bg_texture, (Rectangle) { .width = gameplay->room_width, .height = gameplay->room_height }, room_top_left, WHITE);
}
