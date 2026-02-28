#pragma once

typedef struct Game Game;

typedef struct MainMenu {
	int value;
} MainMenu;

void MainMenu_update(Game* game);
void MainMenu_render(const Game* game);
