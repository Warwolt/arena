#pragma once

#include "resource.h"

#include <raylib.h>

typedef enum ComponentType {
	ComponentType_None,
	ComponentType_Sprite,
	ComponentType_CollisionShape,
} ComponentType;

typedef struct Sprite {
	TextureID texture_id;
	Rectangle clip_rect;
} Sprite;

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
