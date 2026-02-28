#pragma once

#include "engine/resource.h"

typedef struct Engine {
	ResourceManager resources;
	int screen_width;
	int screen_height;
} Engine;
