#pragma once

#include <raylib.h>

typedef struct Circle {
	Vector2 center;
	float radius;
} Circle;

typedef enum ShapeType {
	ShapeType_Circle,
} ShapeType;

typedef struct Shape {
	ShapeType type;
	union {
		Circle circle;
	};
} Shape;

inline Shape Shape_from_circle(Circle circle) {
	return (Shape) {
		.type = ShapeType_Circle,
		.circle = circle,
	};
}

inline void Shape_set_position(Shape* shape, Vector2 pos) {
	switch (shape->type) {
		case ShapeType_Circle: {
			shape->circle.center = pos;
		} break;
	}
}

inline float Shape_width(const Shape* shape) {
	switch (shape->type) {
		case ShapeType_Circle:
			return shape->circle.radius;
	}
	return 0.0f;
}

inline bool Shape_check_collision(const Shape* lhs, const Shape* rhs) {
	switch (lhs->type) {
		case ShapeType_Circle: {
			const Circle* lhs_circle = &lhs->circle;
			switch (rhs->type) {
				case ShapeType_Circle:
					const Circle* rhs_circle = &rhs->circle;
					return Raylib_CheckCollisionCircles(
						lhs_circle->center,
						lhs_circle->radius,
						rhs_circle->center,
						rhs_circle->radius
					);
			}
		}
	}
	return false;
}
