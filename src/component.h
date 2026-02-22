#pragma once

#include "resource.h"

#include <raylib.h>

typedef struct Sprite {
	TextureID texture_id;
	Rectangle clip_rect;
} Sprite;
