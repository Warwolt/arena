#include "game.h"

#include "platform/win32.h"

#define _AMD64_ 1
#define _M_X64 100
#include <libloaderapi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH 260 // minwindef.h

typedef struct GameLibrary {
	HMODULE handle;
	typeof(&Game_initialize) initialize;
	typeof(&Game_shutdown) shutdown;
	typeof(&Game_update) update;
	typeof(&Game_render) render;
} GameLibrary;

GameLibrary load_library(const char* library_path) {
	HMODULE handle = LoadLibraryA(library_path);
	if (!handle) {
		char message[256] = { 0 };
		snprintf(message, 256, "Couldn't load %s\n", library_path);
		Win32_show_error_message_box(message);
		exit(1);
	}

	return (GameLibrary) {
		.handle = handle,
		.initialize = (typeof(&Game_initialize))GetProcAddress(handle, "Game_initialize"),
		.shutdown = (typeof(&Game_shutdown))GetProcAddress(handle, "Game_shutdown"),
		.update = (typeof(&Game_update))GetProcAddress(handle, "Game_update"),
		.render = (typeof(&Game_render))GetProcAddress(handle, "Game_render"),
	};
}

void free_library(GameLibrary* library) {
	FreeLibrary(library->handle);
}

int main(void) {
	Win32_set_process_dpi_aware();

	/* Load DLL */
	GameLibrary game_lib = { 0 };
	{
		/* Get executable directory */
		char executable_directory[MAX_PATH] = { 0 };
		Win32_get_executable_directory(executable_directory);

		/* Get library path */
		const char* library_name = "Game.dll";
		char library_path[MAX_PATH] = { 0 };
		snprintf(library_path, MAX_PATH, "%s\\%s", executable_directory, library_name);

		/* Check library exists */
		if (!Win32_file_exists(library_path)) {
			char message[256] = { 0 };
			snprintf(message, 256, "Can't find %s\n", library_name);
			Win32_show_error_message_box(message);
			exit(1);
		}

		/* Copy library */
		const char* library_copy_name = "Game-hot-reload.dll";
		char library_copy_path[MAX_PATH] = { 0 };
		snprintf(library_copy_path, MAX_PATH, "%s\\%s", executable_directory, library_copy_name);
		Win32_copy_file(library_path, library_copy_path);

		/* Load copy */
		game_lib = load_library(library_copy_name);
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
	free_library(&game_lib);
	return 0;
}
