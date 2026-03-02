#pragma once

#include "game/scene/debug_physics.h"
#include "game/scene/gameplay.h"
#include "game/scene/main_menu.h"

typedef enum SceneID {
	SceneID_MainMenu,
	SceneID_Gameplay,
	SceneID_DebugPhysics,
} SceneID;

typedef struct Scene {
	SceneID id;
	union {
		MainMenu main_menu;
		Gameplay gameplay;
		DebugPhysics debug_physics;
	};
} Scene;

void Scene_initialize(Game* game, SceneID scene_id);
void Scene_update(Game* game);
void Scene_render(const Game* game);
