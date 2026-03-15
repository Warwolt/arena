#include "game/scene.h"

#include "game.h"

typedef struct SceneVTable {
	void (*initialize)(Game*);
	void (*update)(Game*);
	void (*render)(const Game*);
} SceneVTable;

#define MatchSceneVTable(SceneName)                                                                           \
	case SceneID_##SceneName:                                                                                 \
		return (SceneVTable) {                                                                                \
			.initialize = SceneName##_initialize, .update = SceneName##_update, .render = SceneName##_render, \
		}

static SceneVTable scene_vtable(SceneID scene_id) {
	switch (scene_id) {
		MatchSceneVTable(MainMenu);
		MatchSceneVTable(Gameplay);
		MatchSceneVTable(DebugScene);
		MatchSceneVTable(CollisionDebugScene);
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
