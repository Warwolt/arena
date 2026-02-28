#pragma once

#include <stdbool.h>

typedef struct Game Game;

typedef enum SceneID {
	SceneID_MainMenu,
	SceneID_Gameplay,
} SceneID;

typedef struct MainMenu {
	int value;
} MainMenu;

typedef struct Gameplay {
	int value;
} Gameplay;

typedef struct Scene {
	SceneID id;
	union {
		MainMenu main_menu;
		Gameplay gameplay;
	};
} Scene;

void MainMenu_update(Game* game);
void MainMenu_render(const Game* game);

void Gameplay_update(Game* game);
void Gameplay_render(const Game* game);
