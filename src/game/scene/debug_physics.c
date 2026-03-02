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
	const Rectangle screen_rect = Game_screen_rect(game);
	const char* text = "Physics";
	const int font_size = 64;
	int text_width = MeasureText(text, font_size);
	DrawText("Physics", (screen_rect.width - text_width) / 2, (screen_rect.height - font_size) / 2, font_size, WHITE);
}
