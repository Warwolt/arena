#pragma once

#include "core/geometry.h"
#include "engine/resource.h"

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
