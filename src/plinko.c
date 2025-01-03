#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "global.h"
#include "screens/game.h"

int main () {
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT);

	// Create the window
	InitWindow(480, 854, "Plinko");
	// ToggleFullscreen();
	SetTargetFPS(60);
	Image windowIcon = LoadImage("resources/icon.png");
	SetWindowIcon(windowIcon);
	SetExitKey(KEY_P);
	Font defaultFont = LoadFontEx("resources/fonts/arial.ttf", 96, 0, 0);

	// Start the main game loop and enter the title screen
	enum Screen curr_screen = GAME;
	while (!WindowShouldClose()) {
		switch (curr_screen) {
			case GAME:
				curr_screen = GameScreen(defaultFont);
				break;
			default:
				curr_screen = CLOSE_GAME;
				break;
		}

		if (curr_screen == CLOSE_GAME) {
			break;
		}
	}

	// Cleanup
	CloseWindow();
	return 0;
}