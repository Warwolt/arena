#include "game/scene.h"

#include "game.h"

typedef struct SceneneVTable {
	void (*initialize)(Game*);
	void (*update)(Game*);
	void (*render)(const Game*);
} SceneneVTable;

static SceneneVTable scene_vtable(SceneID scene_id) {
	switch (scene_id) {
		case SceneID_MainMenu:
			return (SceneneVTable) {
				.initialize = MainMenu_initialize,
				.update = MainMenu_update,
				.render = MainMenu_render,
			};
		case SceneID_Gameplay:
			return (SceneneVTable) {
				.initialize = Gameplay_initialize,
				.update = Gameplay_update,
				.render = Gameplay_render,
			};
		case SceneID_DebugPhysics:
			return (SceneneVTable) {
				.initialize = DebugPhysics_initialize,
				.update = DebugPhysics_update,
				.render = DebugPhysics_render,
			};
	}
	return (SceneneVTable) { 0 };
}

void Scene_initialize(Game* game, SceneID scene_id) {
	game->scene = (Scene) { .id = scene_id };
	SceneneVTable vtable = scene_vtable(game->scene.id);
	vtable.initialize(game);
}

void Scene_update(Game* game) {
	SceneneVTable vtable = scene_vtable(game->scene.id);
	vtable.update(game);
}

void Scene_render(const Game* game) {
	SceneneVTable vtable = scene_vtable(game->scene.id);
	vtable.render(game);
}
