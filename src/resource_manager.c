#include "resource_manager.h"

#include "logging.h"

TextureID ResourceManager_load_texture(ResourceManager* resources, const char* filename) {
	/* Load image */
	Image image = LoadImage(filename);
	if (image.data == NULL) {
		LOG_ERROR("Couldn't load texture \"%s\"\n", filename);
		return (TextureID) { 0 };
	}

	/* Load and store texture */
	TextureID id = { ++resources->next_id };
	Texture2D texture = LoadTextureFromImage(image);
	Map_insert(&resources->textures, id.value, texture);
	UnloadImage(image);

	return id;
}

bool ResourceManager_get_texture(ResourceManager* resources, TextureID id, Texture* texture) {
	return Map_get(&resources->textures, id.value, texture);
}
