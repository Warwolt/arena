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
} DebugPage;

const char* debug_page_title[] = {
	[DebugPage_CircleCircle] = "Circle Circle",
	[DebugPage_CircleRectangle] = "Circle Rectangle",
};

static void change_scene_page(CollisionDebugScene* scene, DebugPage page) {
	const float shape_size = 100;
	*scene = (CollisionDebugScene) { 0 };
	scene->page = page;
	switch (page) {
		case DebugPage_CircleCircle:
			scene->shapes[scene->num_shapes++] = Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 });
			scene->shapes[scene->num_shapes++] = Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 });
			break;

		case DebugPage_CircleRectangle:
			scene->shapes[scene->num_shapes++] = Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 });
			scene->shapes[scene->num_shapes++] = Shape_from_rectangle((Rectangle) { .x = 0, .y = 0, .width = shape_size, .height = shape_size });
			break;

		case DebugPage_Count:
			break;
	}
}

void CollisionDebugScene_initialize(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	change_scene_page(scene, DebugPage_CircleCircle);
}

void CollisionDebugScene_update(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	scene->time_now += Raylib_GetFrameTime();

	if (Raylib_IsKeyPressed(KEY_ESCAPE)) {
		Game_quit(game);
	}

	if (Raylib_IsKeyPressed(KEY_RIGHT)) {
		change_scene_page(scene, (DebugPage_Count + scene->page + 1) % DebugPage_Count);
		return;
	}

	if (Raylib_IsKeyPressed(KEY_LEFT)) {
		change_scene_page(scene, (DebugPage_Count + scene->page - 1) % DebugPage_Count);
		return;
	}

	// FIXME: Debug why DebugPage_CircleRectangle isn't working correctly.
	//
	// Make the shapes follow the mouse cursor position.
	// It'll be easier to debug stuff if we can move the shape manually.
	// Right now circle-rectangle case isn't working right, some positioning is off (by some offset).
	//
	// We need a resolution relative mouse position.
	// This means taking scaling into account, and taking letterboxing into account.
	// Right now we're just computing that info when rendering, but we should move it into the game update.
	// (Probably make it part of the Window struct?)
	//
	// Vector2 Window_relative_cursor_position(const Window* window, Vector2 global_mouse_pos);
	//
	// input.mouse_pos = Window_relative_cursor_position(&window, Raylib_GetMousePosition())

	if (scene->num_shapes >= 2) {
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
		const Color color = scene->is_overlapping ? RED : GREEN;
		switch (shape->type) {
			case ShapeType_Circle: {
				const Circle* circle = &shape->circle;
				const Vector2 center = Vector2Add(circle->center, screen_middle);
				Raylib_DrawCircleV(center, circle->radius, Raylib_ColorAlpha(color, 0.5f));
				Raylib_DrawCircleLinesV(center, circle->radius, color);
			} break;

			case ShapeType_Rectangle: {
				const Rectangle* rectangle = &shape->rectangle;
				Vector2 position = {
					.x = screen_middle.x + rectangle->x - rectangle->width / 2,
					.y = screen_middle.y + rectangle->y - rectangle->height / 2,
				};
				Raylib_DrawRectangle(position.x, position.y, rectangle->width, rectangle->height, Raylib_ColorAlpha(color, 0.5f));
				Raylib_DrawRectangleLines(position.x, position.y, rectangle->width, rectangle->height, color);
			} break;
		}
	}
}
