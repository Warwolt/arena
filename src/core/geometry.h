#pragma once

#include <raylib.h>

typedef struct Circle {
	Vector2 center;
	float radius;
} Circle;

typedef enum ShapeType {
	ShapeType_Circle,
	ShapeType_Rectangle,
} ShapeType;

typedef struct Shape {
	ShapeType type;
	union {
		Circle circle;
		Rectangle rectangle;
	};
} Shape;

Shape Shape_from_circle(Circle circle);
Shape Shape_from_rectangle(Rectangle rectangle);

// position is relative to center of shape
void Shape_set_position(Shape* shape, Vector2 pos);

float Shape_width(const Shape* shape);
bool Shape_is_overlapping_shape(const Shape* lhs, const Shape* rhs);
