#include "game/scene/gameplay.h"

#include "game/game.h"
#include "platform/logging.h"

#include <raylib.h>
#include <raymath.h>

void Gameplay_initialize(Game* game) {
	LOG_DEBUG("Gameplay initialize");
}

void Gameplay_update(Game* game) {
	if (IsKeyPressed(KEY_ESCAPE)) {
		Game_switch_scene(game, SceneID_MainMenu);
	}
}

void Gameplay_render(const Game* game) {
	ClearBackground(BLACK);
	const int font_size = 64;
	const char* text = "Gameplay";
	int text_width = MeasureText(text, font_size);
	DrawText(text, (game->screen.texture.width - text_width) / 2, (game->screen.texture.height - font_size) / 2, font_size, WHITE);
}
