#include "core/geometry.h"

Rectangle Rectangle_centered_at(Vector2 center, int width, int height) {
	return (Rectangle) {
		.x = center.x - width / 2,
		.y = center.y - height / 2,
		.width = width,
		.height = height,
	};
}

Shape Shape_from_circle(Circle circle) {
	return (Shape) {
		.type = ShapeType_Circle,
		.circle = circle,
	};
}

Shape Shape_from_rectangle(Rectangle rectangle) {
	return (Shape) {
		.type = ShapeType_Rectangle,
		.rectangle = rectangle,
	};
}

float Shape_width(const Shape* shape) {
	switch (shape->type) {
		case ShapeType_Circle:
			return 2 * shape->circle.radius;

		case ShapeType_Rectangle:
			return shape->rectangle.width;
	}
	return 0.0f;
}

// position is relative to center of shape
void Shape_set_position(Shape* shape, Vector2 pos) {
	switch (shape->type) {
		case ShapeType_Circle: {
			Circle* circle = &shape->circle;
			circle->center = pos;
		} break;

		case ShapeType_Rectangle: {
			Rectangle* rectangle = &shape->rectangle;
			rectangle->x = pos.x - rectangle->width / 2;
			rectangle->y = pos.y - rectangle->height / 2;
		} break;
	}
}

bool Shape_is_overlapping_shape(const Shape* lhs, const Shape* rhs) {
	switch (lhs->type) {
		case ShapeType_Circle: {
			const Circle* lhs_circle = &lhs->circle;
			switch (rhs->type) {
				case ShapeType_Circle: {
					const Circle* rhs_circle = &rhs->circle;
					return Raylib_CheckCollisionCircles(
						lhs_circle->center,
						lhs_circle->radius,
						rhs_circle->center,
						rhs_circle->radius
					);
				} break;

				case ShapeType_Rectangle: {
					const Rectangle* rhs_rectangle = &rhs->rectangle;
					return Raylib_CheckCollisionCircleRec(lhs_circle->center, lhs_circle->radius, *rhs_rectangle);
				} break;
			}
		} break;

		case ShapeType_Rectangle: {
			const Rectangle* lhs_rectangle = &lhs->rectangle;
			switch (rhs->type) {
				case ShapeType_Circle: {
					const Circle* rhs_circle = &rhs->circle;
					return Raylib_CheckCollisionCircleRec(rhs_circle->center, rhs_circle->radius, *lhs_rectangle);
				}

				case ShapeType_Rectangle: {
					const Rectangle* rhs_rectangle = &rhs->rectangle;
					return Raylib_CheckCollisionRecs(*lhs_rectangle, *rhs_rectangle);
				}
			}

		} break;
	}
	return false;
}
