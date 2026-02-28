#pragma once

typedef struct Game Game;

typedef struct MainMenu {
	int focused_menu_item;
} MainMenu;

void MainMenu_initialize(Game* game);
void MainMenu_update(Game* game);
void MainMenu_render(const Game* game);
