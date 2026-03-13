#include "game/scene/debug/collision_debug.h"

#include "game.h"

#include <raylib.h>
#include <raymath.h>

void CollisionDebugScene_initialize(Game* game) {
}

void CollisionDebugScene_update(Game* game) {
	if (IsKeyPressed(KEY_ESCAPE)) {
		// Game_switch_scene(game, SceneID_DebugScene);
		Game_quit(game);
	}
}

void CollisionDebugScene_render(const Game* game) {
	const int screen_width = game->screen.texture.width;
	const int screen_height = game->screen.texture.height;
	Vector2 screen_middle = {
		.x = game->screen.texture.width / 2,
		.y = game->screen.texture.height / 2,
	};

	// render two overlapping circles
	ClearBackground(BLACK);
	Game_draw_text(game, "Collision Debug", 0, 0, 32, WHITE);

	float radius = 100;
	Circle circles[2] = {
		(Circle) { .center = { .x = -radius, .y = 0 }, .radius = radius },
		(Circle) { .center = { .x = +radius, .y = 0 }, .radius = radius },
	};

	for (int i = 0; i < sizeof(circles) / sizeof(*circles); i++) {
		const Circle* circle = &circles[i];
		Vector2 center = Vector2Add(circle->center, screen_middle);
		DrawCircleV(center, circle->radius, ColorAlpha(GREEN, 0.5f));
		DrawCircleLinesV(center, circle->radius, GREEN);
	}
}
