#include "resource_manager.h"

#include "logging.h"

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
	TextureID id = { ++resources->next_texture_id };
	Texture2D texture = LoadTextureFromImage(image);
	Map_insert(&resources->textures, id.value, texture);
	UnloadImage(image);

	return id;
}

bool ResourceManager_get_texture(ResourceManager* resources, TextureID id, Texture* texture) {
	return Map_get(&resources->textures, id.value, texture);
}
