#include "game/scene/debug_physics.h"

#include "game.h"

#include <raylib.h>

void DebugPhysics_initialize(Game* game) {
}

void DebugPhysics_update(Game* game) {
	if (IsKeyDown(KEY_ESCAPE)) {
		Game_quit(game);
	}
}

void DebugPhysics_render(const Game* game) {
	const char* text = "Physics";
	const int font_size = 64;
	int text_width = MeasureText(text, font_size);
	DrawText("Physics", (game->screen.texture.width - text_width) / 2, (game->screen.texture.height - font_size) / 2, font_size, WHITE);
}
