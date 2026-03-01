#include "game.h"

#include "platform/win32.h"

#define _AMD64_ 1
#define _M_X64 100
#include <libloaderapi.h>
#include <stdio.h>

typedef struct GameLibrary {
	HMODULE handle;
	typeof(&Game_initialize) initialize;
	typeof(&Game_shutdown) shutdown;
	typeof(&Game_update) update;
	typeof(&Game_render) render;
} GameLibrary;

GameLibrary load_library(const char* library_path) {
	HMODULE handle = LoadLibraryA(library_path);
	return (GameLibrary) {
		.handle = handle,
		.initialize = (typeof(&Game_initialize))GetProcAddress(handle, "Game_initialize"),
		.shutdown = (typeof(&Game_shutdown))GetProcAddress(handle, "Game_shutdown"),
		.update = (typeof(&Game_update))GetProcAddress(handle, "Game_update"),
		.render = (typeof(&Game_render))GetProcAddress(handle, "Game_render"),
	};
}

int main(void) {
	/* Load DLL */
	const char* library_path = "Game.dll";
	if (!Win32_file_exists_relative_executable(library_path)) {
		char message[256] = { 0 };
		snprintf(message, 256, "Can't find %s\n", library_path);
		Win32_show_error_message_box(message);
		return 1;
	}
	GameLibrary game_lib = load_library(library_path);
	if (!game_lib.handle) {
		fprintf(stderr, "Fatal: Couldn't load library \"%s\"!\n", library_path);
		return 1;
	}

	/* State */
	Game game_state = { 0 };
	game_lib.initialize(&game_state);

	/* Run program */
	while (!game_state.should_quit) {
		/* Check hot reload */
		if (IsKeyPressed(KEY_F5)) {
			printf("F5\n");
		}

		/* Run game frame */
		game_lib.update(&game_state);
		game_lib.render(&game_state);
	}

	/* Shutdown */
	game_lib.shutdown(&game_state);
	FreeLibrary(game_lib.handle);
	return 0;
}
