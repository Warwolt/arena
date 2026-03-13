#pragma once

#include <raylib.h>

typedef enum ShapeType {
	ShapeType_Circle,
} ShapeType;

typedef struct Circle {
	Vector2 center;
	float radius;
} Circle;

typedef struct Shape {
	ShapeType type;
	Vector2 position; // relative to entity
	union {
		Circle circle;
	};
} Shape;

inline Shape Shape_circle(Circle circle) {
	return (Shape) {
		.type = ShapeType_Circle,
		.circle = circle,
	};
}
