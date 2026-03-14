#include "game/scene/debug/collision_debug.h"

#include "game.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

typedef enum DebugPage {
	DebugPage_CircleCircle,
	DebugPage_CircleRectangle,
	DebugPage_Count,
} CollisionDebug_Page;

const char* debug_page_title[] = {
	[DebugPage_CircleCircle] = "Circle Circle",
	[DebugPage_CircleRectangle] = "Circle Rectangle",
};

void CollisionDebugScene_initialize(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	const float radius = 100;
	*scene = (CollisionDebugScene){
		.circles = {
			(Circle) { .center = { .x = 0, .y = 0 }, .radius = radius },
			(Circle) { .center = { .x = 0, .y = 0 }, .radius = radius },
		},
	};
}

void CollisionDebugScene_update(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	scene->time_now += GetFrameTime();

	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		scene->page = (DebugPage_Count + scene->page + 1) % DebugPage_Count;
	}

	if (IsKeyPressed(KEY_LEFT)) {
		scene->page = (DebugPage_Count + scene->page - 1) % DebugPage_Count;
	}

	// TODO:
	// Add a circle-rectangle case
	// It would be nice if we could just throw in some shapes during init of the given page, and
	// then let this update code just run on "shapes". Whatever we configure we should be able to
	// move one of the shapes back and forth, and check for overlaps.

	Circle* circle1 = &scene->circles[0];
	Circle* circle2 = &scene->circles[1];

	// oscillate first circle
	float period = 6.0f; // seconds
	float freq = 1 / period;
	float amplitude = 3 * scene->circles[0].radius;
	scene->circles[0].center.x = roundf(sinf(scene->time_now * freq * 2 * PI) * amplitude);

	// check overlap
	scene->is_overlapping = CheckCollisionCircles(circle1->center, circle1->radius, circle2->center, circle2->radius);
}

void CollisionDebugScene_render(const Game* game) {
	const CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	const Vector2 screen_middle = {
		.x = game->screen.texture.width / 2,
		.y = game->screen.texture.height / 2,
	};

	ClearBackground(BLACK);
	Game_draw_text(game, "Collision Debug", 1, 1, 16, WHITE);
	char text[128];
	snprintf(text, 128, "Page %d/%d: %s", scene->page + 1, DebugPage_Count, debug_page_title[scene->page]);
	Game_draw_text(game, text, 1, 1 + 16, 16, WHITE);
	for (int i = 0; i < sizeof(scene->circles) / sizeof(*scene->circles); i++) {
		const Circle* circle = &scene->circles[i];
		const Vector2 center = Vector2Add(circle->center, screen_middle);
		const Color color = scene->is_overlapping ? RED : GREEN;
		DrawCircleV(center, circle->radius, ColorAlpha(color, 0.5f));
		DrawCircleLinesV(center, circle->radius, color);
	}
}
