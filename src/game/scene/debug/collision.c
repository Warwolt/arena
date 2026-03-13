#include "game/scene/debug/collision.h"

#include "game.h"

void CollisionDebugScene_initialize(Game* game) {
}

void CollisionDebugScene_update(Game* game) {
	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_switch_scene(game, SceneID_DebugScene);
	}
}

void CollisionDebugScene_render(const Game* game) {
	const int screen_width = game->screen.texture.width;
	const int screen_height = game->screen.texture.height;

	// render two overlapping circles
	ClearBackground(BLACK);
	Game_draw_text(game, "Collision Debug", 0, 0, 16, WHITE);

	DrawCircle(screen_width / 2, screen_height / 2, 10, GREEN);
}
