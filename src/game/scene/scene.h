#pragma once

#include "game/scene/gameplay.h"
#include "game/scene/main_menu.h"

typedef enum SceneID {
	SceneID_MainMenu,
	SceneID_Gameplay,
} SceneID;

typedef struct Scene {
	SceneID id;
	union {
		MainMenu main_menu;
		Gameplay gameplay;
	};
} Scene;
