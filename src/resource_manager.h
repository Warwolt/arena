#pragma once

#include "map.h"

#include <raylib.h>

#define MAX_TEXTURE_RESOURCES (int)256

typedef struct ResourceManager {
	Map(Texture2D, MAX_TEXTURE_RESOURCES) textures;
	int next_id;
} ResourceManager;

typedef struct TextureID {
	int value;
} TextureID;

TextureID ResourceManager_load_texture(ResourceManager* resources, const char* filename);
bool ResourceManager_get_texture(ResourceManager* resources, TextureID id, Texture* texture);
