#include "engine/resource.h"

#include "platform/logging.h"

#define DEBUG_RESOURCE_MANAGER true

TextureID ResourceManager_load_texture(ResourceManager* resources, const char* filename) {
	if (resources->textures.size == MAX_TEXTURE_RESOURCES) {
		LOG_ERROR("Can't load texture \"%s\", too many already loaded (%d)", filename, MAX_TEXTURE_RESOURCES);
		return (TextureID) { 0 };
	}

	/* Load image */
	Image image = LoadImage(filename);
	if (image.data == NULL) {
		LOG_ERROR("Couldn't load texture \"%s\"", filename);
		return (TextureID) { 0 };
	}

	/* Load and store texture */
	TextureID id = { ++resources->prev_texture_id };
	Texture2D texture = LoadTextureFromImage(image);
	SparseArray_insert(&resources->textures, id.value, texture);
	UnloadImage(image);

	if (DEBUG_RESOURCE_MANAGER) {
		LOG_DEBUG("Loaded texture \"%s\" with id %d", filename, id.value);
	}

	return id;
}

bool ResourceManager_get_texture(const ResourceManager* resources, TextureID id, Texture* texture) {
	return SparseArray_get(&resources->textures, id.value, texture);
}

void ResourceManager_unload_resources(ResourceManager* resources) {
	/* Unload textures */
	resources->prev_texture_id = 0;
	for (size_t i = 0; i < resources->textures.size; i++) {
		UnloadTexture(resources->textures.values[i]);
		SparseArray_remove(&resources->textures, resources->textures.keys[i]);
	}
}
