#include "game/scene.h"
#include "game/game.h"

#include <raylib.h>
#include <raymath.h>

// FIXME move to struct field
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

void MainMenu_update(Game* game) {
	if (IsKeyPressed(KEY_ENTER)) {
		game->scene = (Scene) { .id = SceneID_Gameplay };
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		game->should_quit = true;
	}
}

void MainMenu_render(const Game* game) {
	ClearBackground(PURPLE);
	const int font_size = 64;
	const char* text = "Main Menu";
	int text_width = MeasureText(text, font_size);
	DrawText(text, (game->engine.screen_width - text_width) / 2, (game->engine.screen_height - font_size) / 2, font_size, WHITE);
}

void Gameplay_update(Game* game) {
	if (IsKeyPressed(KEY_ESCAPE)) {
		game->scene = (Scene) { .id = SceneID_MainMenu };
	}
}

void Gameplay_render(const Game* game) {
	ClearBackground(GREEN);
	const int font_size = 64;
	const char* text = "Gameplay";
	int text_width = MeasureText(text, font_size);
	DrawText(text, (game->engine.screen_width - text_width) / 2, (game->engine.screen_height - font_size) / 2, font_size, WHITE);
}
