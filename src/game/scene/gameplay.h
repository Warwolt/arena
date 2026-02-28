#pragma once

typedef struct Game Game;

typedef struct Gameplay {
	int value;
} Gameplay;

void Gameplay_update(Game* game);
void Gameplay_render(const Game* game);
