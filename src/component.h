#pragma once

#include "resource.h"

#include <raylib.h>

typedef enum ComponentType {
	ComponentType_None = 0,
	ComponentType_Position = 1,
	ComponentType_Sprite = 2,
} ComponentType;

typedef struct Sprite {
	TextureID texture_id;
	Rectangle clip_rect;
} Sprite;
