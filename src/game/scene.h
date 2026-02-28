#pragma once

typedef struct GameState GameState;

typedef enum SceneID {
	SceneID_MainMenu,
	SceneID_Gameplay,
} SceneID;

void MainMenu_update(GameState* game);
void MainMenu_render(GameState* game);

void Gameplay_update(GameState* game);
void Gameplay_render(GameState* game);
