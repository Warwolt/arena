#include "game/scene/debug/collision_debug.h"

#include "game.h"
#include "platform/assert.h"

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
		.shapes = {
			Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = radius }),
			Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = radius }),
		},
	};
}

void CollisionDebugScene_update(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	scene->time_now += Raylib_GetFrameTime();

	if (Raylib_IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}

	if (Raylib_IsKeyPressed(KEY_RIGHT)) {
		scene->page = (DebugPage_Count + scene->page + 1) % DebugPage_Count;
	}

	if (Raylib_IsKeyPressed(KEY_LEFT)) {
		scene->page = (DebugPage_Count + scene->page - 1) % DebugPage_Count;
	}

	Shape* shape1 = &scene->shapes[0];
	Shape* shape2 = &scene->shapes[1];

	/* Oscillate first shape */
	const float period = 6.0f; // seconds
	const float freq = 1 / period;
	const float amplitude = 3 * Shape_width(shape1);
	Shape_set_position(shape1, (Vector2) { roundf(sinf(scene->time_now * freq * 2 * PI) * amplitude), 0 });

	/* Check collisions */
	scene->is_overlapping = Shape_check_collision(shape1, shape2);
}

void CollisionDebugScene_render(const Game* game) {
	const CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	const Vector2 screen_middle = {
		.x = game->screen.texture.width / 2,
		.y = game->screen.texture.height / 2,
	};

	Raylib_ClearBackground(BLACK);
	Game_draw_text(game, "Collision Debug", 1, 1, 16, WHITE);
	char text[128];
	snprintf(text, 128, "Page %d/%d: %s", scene->page + 1, DebugPage_Count, debug_page_title[scene->page]);
	Game_draw_text(game, text, 1, 1 + 16, 16, WHITE);
	for (int i = 0; i < sizeof(scene->shapes) / sizeof(*scene->shapes); i++) {
		const Shape* shape = &scene->shapes[i];
		switch (shape->type) {
			case ShapeType_Circle: {
				const Circle* circle = &shape->circle;
				const Vector2 center = Vector2Add(circle->center, screen_middle);
				const Color color = scene->is_overlapping ? RED : GREEN;
				Raylib_DrawCircleV(center, circle->radius, Raylib_ColorAlpha(color, 0.5f));
				Raylib_DrawCircleLinesV(center, circle->radius, color);
			} break;
		}
	}
}
