#include "game/scene/debug.h"

#include "game.h"

#include <raylib.h>

#define MIDNIGHT_COMMANDER CLITERAL(Color) { 0, 0, 187, 255 }

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum DebugMenuItem {
	DebugMenuItem_Physics,
	DebugMenuItem_ItemTwo,
	DebugMenuItem_ItemThree,
	DebugMenuItem_Count,
} DebugMenuItem;

typedef enum DebugPage {
	DebugPage_Menu,
	DebugPage_Physics,
} DebugPage;

static const char* menu_item_names[DebugMenuItem_Count] = {
	[DebugMenuItem_Physics] = "1. Physics",
	[DebugMenuItem_ItemTwo] = "2. ItemTwo",
	[DebugMenuItem_ItemThree] = "3. ItemThree",
};

// FIXME:
//
// Ideally we should be able to quite freely add a lot of debug pages.
// Repeating the same menu item selection code over and over isn't great.
//
// Can we write some incredibly simple UI code?
//
// It would be great to be able to do some kind of ImGui style declaration
// of what menu items we have, and do the selection handling in an if-block.
//
// Something like:
//
//  	UI_begin_menu(ui);
//  	{
//  		if (UI_menu_item(ui, "Physics")) {
//  			LOG_DEBUG("Physics selected");
//  		}
//
//  		if (UI_menu_item(ui, "Audio")) {
//  			LOG_DEBUG("Audio selected");
//  		}
//  	}
//  	UI_end_menu(ui);
//
// Can we maybe even take a page out of how Dear ImGui works and just have a hidden global context
// variable? (How would that work with DLL hot reloading though? Setter function for the context?)
//
//  	UI_begin_menu();
//  	{
//  		if (UI_menu_item("Physics")) {
//  			LOG_DEBUG("Physics selected");
//  		}
//
//  		if (UI_menu_item("Audio")) {
//  			LOG_DEBUG("Audio selected");
//  		}
//  	}
//  	UI_end_menu();
//
// How would we render that later?
// Would be simplest to be able to use the context of where we're in the code to decide on how to
// draw it, and not bother with any flags and parameters or such stuff initially:
//
//		DebugScene_render(const Game* game) {
//			const DebugScene* debug_scene = &game->scene.debug_scene;
//			const UI* ui = UI_get_context();
//			if (debug_scene->current_page == DebugPage_Foo) {
//				// render content of ui one way
//			}
//			if (debug_scene->current_page == DebugPage_Bar) {
//				// render content of ui some other way
//			}
//		}
//
// This _kind of_ works like HTML and CSS works. Content and presentation are kept separate.
// We know in what scene and in what state we rendered the current frame, so we can deal with
// the particulars of presentation based on all that information.
//
// The UI interface only needs to deal with declaring content and event handling (that a menu item
// was pressed). Only if we're dealing with mouse buttons etc. do we need layout information for
// determining if a press event has happend or not.
//
// Handling the UI navigation stuff can probably be done with a `UI_update` function that's called
// inside of game.c

void DebugScene_initialize(Game* game) {
}

void DebugScene_update(Game* game) {
	DebugScene* debug_scene = &game->scene.debug_scene;
	switch (debug_scene->current_page) {
		case DebugPage_Menu: {
			if (IsKeyPressed(KEY_ESCAPE)) {
				Game_quit(game);
			}

			if (IsKeyPressed(KEY_DOWN)) {
				debug_scene->focused_menu_item = (DebugMenuItem_Count + debug_scene->focused_menu_item + 1) % DebugMenuItem_Count;
			}

			if (IsKeyPressed(KEY_UP)) {
				debug_scene->focused_menu_item = (DebugMenuItem_Count + debug_scene->focused_menu_item - 1) % DebugMenuItem_Count;
			}

			if (IsKeyPressed(KEY_ENTER)) {
				if (debug_scene->focused_menu_item == DebugMenuItem_Physics) {
					debug_scene->current_page = DebugPage_Physics;
				}
			}

		} break;

		case DebugPage_Physics: {
			if (IsKeyPressed(KEY_ESCAPE)) {
				debug_scene->current_page = DebugPage_Menu;
			}
		} break;
	}
}

void DebugScene_render(const Game* game) {
	const DebugScene* debug_scene = &game->scene.debug_scene;
	const Rectangle screen_rect = Game_screen_rect(game);
	const int font_size = 32;

	ClearBackground(MIDNIGHT_COMMANDER);

	switch (debug_scene->current_page) {
		case DebugPage_Menu: {
			// draw outline
			const int x_off = font_size / 4;
			const int y_off = font_size / 2;
			const Rectangle outline = {
				.x = x_off,
				.y = y_off,
				.width = screen_rect.width - 2 * x_off,
				.height = screen_rect.height - 2 * y_off,
			};
			DrawRectangleLinesEx(outline, 2, YELLOW);

			// draw title
			{
				const char* text = "Debug Screen";
				int text_width = Game_measure_text_width(game, text, font_size);
				int text_x = (screen_rect.width - text_width) / 2;
				int text_y = 0;
				DrawRectangle(text_x - 4, text_y, text_width + 8, font_size, MIDNIGHT_COMMANDER);
				Game_draw_text(game, text, text_x, text_y, font_size, YELLOW);
			}

			// draw menu items
			{
				int list_height = DebugMenuItem_Count * font_size;
				int list_width = 0;
				for (int i = 0; i < DebugMenuItem_Count; i++) {
					int text_width = Game_measure_text_width(game, menu_item_names[i], font_size);
					list_width = max(list_width, text_width);
				}

				for (int i = 0; i < DebugMenuItem_Count; i++) {
					const char* text = menu_item_names[i];
					int text_x = (screen_rect.width - list_width) / 2;
					int text_y = (screen_rect.height - list_height) / 2 + i * font_size;
					bool is_focused = i == debug_scene->focused_menu_item;
					Color background_color = is_focused ? WHITE : MIDNIGHT_COMMANDER;
					Color text_color = is_focused ? MIDNIGHT_COMMANDER : WHITE;
					DrawRectangle(text_x - 1, text_y, list_width + 1, font_size, background_color);
					Game_draw_text(game, text, text_x, text_y, font_size, text_color);
				}
			}
		} break;

		case DebugPage_Physics: {
			// draw outline
			const int x_off = font_size / 4;
			const int y_off = font_size / 2;
			const Rectangle outline = {
				.x = x_off,
				.y = y_off,
				.width = screen_rect.width - 2 * x_off,
				.height = screen_rect.height - 2 * y_off,
			};
			DrawRectangleLinesEx(outline, 2, YELLOW);

			// draw title
			{
				const char* text = "Physics";
				int text_width = Game_measure_text_width(game, text, font_size);
				int text_x = (screen_rect.width - text_width) / 2;
				int text_y = 0;
				DrawRectangle(text_x - 4, text_y, text_width + 8, font_size, MIDNIGHT_COMMANDER);
				Game_draw_text(game, text, text_x, text_y, font_size, YELLOW);
			}
		} break;
	}
}
