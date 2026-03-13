#include "game/scene.h"

#include "game.h"

typedef struct SceneVTable {
	void (*initialize)(Game*);
	void (*update)(Game*);
	void (*render)(const Game*);
} SceneVTable;

static SceneVTable scene_vtable(SceneID scene_id) {
	switch (scene_id) {
		case SceneID_MainMenu:
			return (SceneVTable) {
				.initialize = MainMenu_initialize,
				.update = MainMenu_update,
				.render = MainMenu_render,
			};
		case SceneID_Gameplay:
			return (SceneVTable) {
				.initialize = Gameplay_initialize,
				.update = Gameplay_update,
				.render = Gameplay_render,
			};
		case SceneID_DebugScene:
			return (SceneVTable) {
				.initialize = DebugScene_initialize,
				.update = DebugScene_update,
				.render = DebugScene_render,
			};
		case SceneID_CollisionDebugScene:
			return (SceneVTable) {
				.initialize = CollisionDebugScene_initialize,
				.update = CollisionDebugScene_update,
				.render = CollisionDebugScene_render,
			};
	}
	return (SceneVTable) { 0 };
}

void Scene_initialize(Game* game, SceneID scene_id) {
	game->scene = (Scene) { .id = scene_id };
	SceneVTable scene = scene_vtable(game->scene.id);
	scene.initialize(game);
}

void Scene_update(Game* game) {
	SceneVTable scene = scene_vtable(game->scene.id);
	scene.update(game);
}

void Scene_render(const Game* game) {
	SceneVTable scene = scene_vtable(game->scene.id);
	scene.render(game);
}
