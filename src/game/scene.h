#pragma once

#include "game/scene/debug.h"
#include "game/scene/debug/collision_debug.h"
#include "game/scene/gameplay.h"
#include "game/scene/main_menu.h"

typedef enum SceneID {
	SceneID_MainMenu,
	SceneID_Gameplay,
	SceneID_DebugScene,
	SceneID_CollisionDebugScene,
} SceneID;

typedef struct Scene {
	SceneID id;
	union {
		MainMenu main_menu;
		Gameplay gameplay;
		DebugScene debug_scene;
		CollisionDebugScene collision_debug_scene;
	};
} Scene;

void Scene_initialize(Game* game, SceneID scene_id);
void Scene_update(Game* game);
void Scene_render(const Game* game);
