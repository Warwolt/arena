#include "game/scene/debug/collision_debug.h"

#include "game.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define SHAPE_NONE ((int)-1)

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
	*scene = (CollisionDebugScene) { .mouse_grabed_shape = SHAPE_NONE };
	scene->page = page;
	switch (page) {
		case DebugPage_CircleCircle:
			ShapeArray_push(&scene->shapes, Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 }));
			ShapeArray_push(&scene->shapes, Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 }));
			ShapeArray_push(&scene->shapes, Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 }));
			break;

		case DebugPage_CircleRectangle:
			ShapeArray_push(&scene->shapes, Shape_from_circle((Circle) { .center = { .x = 0, .y = 0 }, .radius = shape_size / 2 }));
			ShapeArray_push(&scene->shapes, Shape_from_rectangle(Rectangle_centered_at((Vector2) { .x = 0, .y = 0 }, shape_size, shape_size)));
			ShapeArray_push(&scene->shapes, Shape_from_rectangle(Rectangle_centered_at((Vector2) { .x = 0, .y = 0 }, shape_size, shape_size)));
			break;

		case DebugPage_Count:
			break;
	}
}

static void draw_debug_shape(const Game* game, const Shape* shape, Color color) {
	const Vector2 window_middle = {
		.x = Window_width(&game->window) / 2,
		.y = Window_height(&game->window) / 2,
	};
	switch (shape->type) {
		case ShapeType_Circle: {
			const Circle* circle = &shape->circle;
			const Vector2 center = Vector2Add(circle->center, window_middle);
			Raylib_DrawCircleV(center, circle->radius, Raylib_ColorAlpha(color, 0.5f));
			Raylib_DrawCircleLinesV(center, circle->radius, color);
		} break;

		case ShapeType_Rectangle: {
			const Rectangle* rectangle = &shape->rectangle;
			Vector2 position = {
				.x = window_middle.x + rectangle->x,
				.y = window_middle.y + rectangle->y,
			};
			Raylib_DrawRectangle(position.x, position.y, rectangle->width, rectangle->height, Raylib_ColorAlpha(color, 0.5f));
			Raylib_DrawRectangleLines(position.x, position.y, rectangle->width, rectangle->height, color);
		} break;
	}
}

void CollisionDebugScene_initialize(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	change_scene_page(scene, DebugPage_CircleCircle);
}

void CollisionDebugScene_update(Game* game) {
	CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	scene->time_now += Raylib_GetFrameTime();
	Rectangle window = Window_rectangle(&game->window);
	Vector2 world_mouse_position = {
		.x = game->input.mouse.x - window.width / 2,
		.y = game->input.mouse.y - window.height / 2,
	};

	if (game->input.action_is_pressed[InputAction_Back]) {
		Game_quit(game);
	}

	if (game->input.action_is_pressed[InputAction_Right]) {
		change_scene_page(scene, (DebugPage_Count + scene->page + 1) % DebugPage_Count);
		return;
	}

	if (game->input.action_is_pressed[InputAction_Left]) {
		change_scene_page(scene, (DebugPage_Count + scene->page - 1) % DebugPage_Count);
		return;
	}

	/* Move shapes along paths */
	{
		/* Oscillate first shape */
		if (scene->shapes.num_values >= 1 && scene->mouse_grabed_shape != 0) {
			Shape* shape1 = &scene->shapes.values[0];
			const float period = 6.0f; // seconds
			const float freq = 1 / period;
			const float amplitude = 2.2 * Shape_width(shape1);
			Shape_set_position(shape1, (Vector2) { roundf(sinf(scene->time_now * freq * 2 * PI) * amplitude), 0 });
		}

		/* Place second shape in center */
		if (scene->shapes.num_values >= 2 && scene->mouse_grabed_shape != 1) {
			Shape* shape2 = &scene->shapes.values[1];
			Shape_set_position(shape2, Vector2Zero());
		}

		/* Rotate third shape */
		if (scene->shapes.num_values >= 3 && scene->mouse_grabed_shape != 2) {
			Shape* shape3 = &scene->shapes.values[2];
			const float period = 4.0f; // seconds
			const float freq = 1 / period;
			const float amplitude = 1.1 * Shape_width(shape3);
			const float t = scene->time_now * freq * 2 * PI;
			Shape_set_position(shape3, (Vector2) { roundf(cosf(t) * amplitude), roundf(sinf(t) * amplitude) });
		}
	}

	/* Update manually controlled shape */
	{
		/* Grab or let go of shape */
		if (Raylib_IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			/* If not controlling a shape, try to grab currently hovered shape */
			if (scene->mouse_grabed_shape == SHAPE_NONE) {
				for (int i = 0; i < scene->shapes.num_values; i++) {
					Shape* shape = &scene->shapes.values[i];
					bool is_hovering_shape = Shape_is_overlapping_point(shape, world_mouse_position);
					if (is_hovering_shape) {
						scene->mouse_grabed_shape = i;
						break;
					}
				}
			}
			/* If controlling a shape, let go of it */
			else {
				scene->mouse_grabed_shape = SHAPE_NONE;
			}
		}

		/* Update position of grabbed shape */
		if (scene->mouse_grabed_shape != SHAPE_NONE) {
			Shape* shape = &scene->shapes.values[scene->mouse_grabed_shape];
			Shape_set_position(shape, world_mouse_position);
		}
	}

	/* Check collisions */
	memset(scene->shape_is_overlapping, 0, sizeof(scene->shape_is_overlapping));
	for (int i = 0; i < scene->shapes.num_values; i++) {
		for (int j = i + 1; j < scene->shapes.num_values; j++) {
			if (Shape_is_overlapping_shape(&scene->shapes.values[i], &scene->shapes.values[j])) {
				scene->shape_is_overlapping[i] = true;
				scene->shape_is_overlapping[j] = true;
			}
		}
	}
}

void CollisionDebugScene_render(const Game* game) {
	const CollisionDebugScene* scene = &game->scene.collision_debug_scene;
	Raylib_ClearBackground(BLACK);

	/* Draw header */
	{
		Game_draw_text(game, "Collision Debug (Mouse click to take control)", 1, 1, 16, WHITE);
		char text[128];
		snprintf(text, 128, "Page %d/%d: %s", scene->page + 1, DebugPage_Count, debug_page_title[scene->page]);
		Game_draw_text(game, text, 1, 1 + 16, 16, WHITE);
	}

	/* Draw shapes */
	for (int i = 0; i < scene->shapes.num_values; i++) {
		const Shape* shape = &scene->shapes.values[i];
		const Color color = scene->shape_is_overlapping[i] ? RED : GREEN;
		draw_debug_shape(game, shape, color);
	}
}
