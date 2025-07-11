#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "../uielements.h"
#include "../settings.h"
#include "../global.h"
#include "../layouts/settingslayout.h"

#define DEFAULT_PEGS 15
#define PEG_RADIUS 12
#define DROP_RANGE 250

#define ZONE_ANIMATION_FRAMES 8
#define ZONE_ANIMATION_OFFSET 8

#define DEFAULT_BALANCE 99

Font font;

typedef struct Plinko_Ball PBall;
struct Plinko_Ball {
	int x;
	int y;
	int dx;
	int dy;
	int radius;
	PBall* next;
	PBall* prev;
};

int get_drop_location() {
	return RandomInRange(0, DROP_RANGE) + ((GetScreenWidth()-DROP_RANGE)/2);
}

void generate_ball(PBall **tail) {
	PBall *ball = (PBall*) malloc(sizeof(PBall));
	ball->x = get_drop_location();
	ball->y = 100;
	ball->dx = 0;
	ball->dy = 9;
	ball->radius = 7;
	ball->next = NULL;
	ball->prev = (*tail);

	(*tail)->next = ball;
	*tail = ball;
}

void remove_ball(PBall **ball, PBall **tail) {
	PBall *temp = *ball;
	temp->prev->next = temp->next;
	if (temp->next != NULL) { // make sure ball isnt the end of the list
		temp->next->prev = temp->prev;
	} else {
		(*tail) = temp->prev;
	}
	(*ball) = temp->next;
	free(temp);
}

Color ZONE_COLORS[3] = {RED, ORANGE, GREEN};
int zone_width;
int height;
int width;
void draw_zone(int location, int animation_offset, int value) {
	// Make sure the proper color is available
	if (value > sizeof(ZONE_COLORS)/sizeof(ZONE_COLORS[0])) {
		return;
	}

	// Generate the zone label and calculate the offset for the text
	char label[10] = "";
	sprintf(label, "%dx", value);
	int textWidth = MeasureTextEx(font, label, 21, 0).x;

	// Start drawing the zone
	DrawRectangle(location, (height-25) + animation_offset, zone_width, 25, ZONE_COLORS[value]);
	DrawTextEx(font, label, (Vector2) {location+((zone_width-textWidth)/2), (height-23) + animation_offset},
		21, 0, WHITE);
}

/***************************
 * START SETTINGS OVERHAUL *
 ***************************/

AppliedGameSettings g_appliedGameSettings;
SettingsLayout g_settingsLayout;
ResolutionSettings g_resolutionSettings;

/*************************
 * END SETTINGS OVERHAUL *
 *************************/

typedef struct gameSettings {
	char aspectRatio[50];
	char resolution[50];
	ScrollSelector *resolutionSelector;
} GameSettings;

typedef struct gameobjects {
	int pegs[DEFAULT_PEGS][2];
	int zone_location[5];
	int zone_animation_state[5];
} GameObjects;

typedef struct gameui {
	UIButton dropButton;
	UINumberLabel balanceDisplay;
} GameUI;

typedef struct gamestate {
	int balance;
	int paused;
	int zone_width;
	PBall **balls_head;
	PBall **balls_curr;
	PBall **balls_tail;
	GameUI ui;
	GameObjects objects;
	GameSettings settings;
} GameState;

// Init global Game State, this might need to be localized later and probably should be but
// for now this is the best way I can get this to work with my custom UI Library without breaking larger
// programming conventions. Make it first, make it good later
GameState *g_gameState;

void resetScreenPositions(GameState *gameState) {
	/***** UI *****/
	// Init UI
	height = GetScreenHeight();
	width = GetScreenWidth();
	gameState->ui.dropButton.x = ((width/3)*2)+10; // TODO: consolidate this into one function
	gameState->ui.dropButton.y = 10;
	gameState->ui.dropButton.width = (width/3)-20;
	gameState->ui.dropButton.height = 30;

	gameState->ui.balanceDisplay.x = 10; // TODO: consolidate this into one function
	gameState->ui.balanceDisplay.y = 10;
	gameState->ui.balanceDisplay.width = (width/3)-20;
	gameState->ui.balanceDisplay.height = 30;

	// Resize settings stuff
	resize_settingslayout(&g_settingsLayout, width, height);

	// Resize video tab
	for (int i = 0; i < g_settingsLayout.videoTab.num_elements; i++) {
		if (g_settingsLayout.videoTab.layout[i]->type == UIT_SCROLLSELECTOR) {
			updateScrollSelectorPositions(g_settingsLayout.videoTab.layout[i]->element.scrollSelector, font);
		}
	}

	// Resize audio tab
	for (int i = 0; i < g_settingsLayout.audioTab.num_elements; i++) {
		if (g_settingsLayout.audioTab.layout[i]->type == UIT_SCROLLSELECTOR) {
			updateScrollSelectorPositions(g_settingsLayout.audioTab.layout[i]->element.scrollSelector, font);
		}
	}

	// Resize game tab
	for (int i = 0; i < g_settingsLayout.gameTab.num_elements; i++) {
		if (g_settingsLayout.gameTab.layout[i]->type == UIT_SCROLLSELECTOR) {
			updateScrollSelectorPositions(g_settingsLayout.gameTab.layout[i]->element.scrollSelector, font);
		}
	}

	/***** Game Objects *****/
	// Pegs
	int num_in_row = 1;
	int pyramid_start_x = width/2;
	const int pyramid_offset_x = 75;
	const int pyramid_start_y = height/5;
	const int pyramid_offset_y = 50;
	for (int i = 0; i < DEFAULT_PEGS; i++) {
		int row_position = (i+1) - (SumTo(num_in_row) - num_in_row);
		gameState->objects.pegs[i][0] = (pyramid_offset_x * (row_position-1)) + pyramid_start_x;
		gameState->objects.pegs[i][1] = (pyramid_offset_y * num_in_row) + pyramid_start_y;
		if ((i+1) == SumTo(num_in_row)) {
			num_in_row++;
			pyramid_start_x -= pyramid_offset_x/2;
		}
	}

	// Generate zones
	zone_width = (width/5);
	gameState->zone_width = zone_width;
	for (int i = 0; i < 5; i++) {
		gameState->objects.zone_animation_state[i] = 0;
		gameState->objects.zone_location[i] = zone_width * i;
	}
}

int dropButtonCallback() {
	printf("Balance Value: %d\n", g_gameState->balance);
	if (g_gameState->balance > 0) {
		printf("Generating Ball\n");
		generate_ball((g_gameState->balls_tail));
		g_gameState->balance -= 1;
	}
	return 0;
}

int pauseApplyButtonCallback() {
	Resolution newResolution = g_appliedGameSettings.videoSettings.selectedResolution;
	printf("Updating window size to %s. Actual Values: %dx%d\n", newResolution.label,
		newResolution.width, newResolution.height);
	SetWindowSize(newResolution.width, newResolution.height);
	resetScreenPositions(g_gameState);
	return 0;
}

int pauseCloseButtonCallback() {
	printf("Closing pause menu.\n");
	g_gameState->paused = 0;
	return 0;
}

int resolutionOnChange(char *selection) {
	printf("Updating the resolution in g_appliedGameSettings to %s.\n", selection);
	g_appliedGameSettings.videoSettings.selectedResolution = get_resolution(g_resolutionSettings,
		g_appliedGameSettings.videoSettings.selectedAspectRatio, selection);
	return 0;
}

int aspectOnChange(char *selection) { // TODO FIX
	if (strcmp(selection, "9:16") == 0) {
		g_appliedGameSettings.videoSettings.selectedAspectRatio = NINE_BY_SIXTEEN;
		g_appliedGameSettings.videoSettings.selectedResolution = g_resolutionSettings.nineBySixteen[0];
	} else {
		g_appliedGameSettings.videoSettings.selectedAspectRatio = SIXTEEN_BY_NINE;
		g_appliedGameSettings.videoSettings.selectedResolution = g_resolutionSettings.sixteenByNine[0];
	}

	// Update the resolution selector
	int t_numLabels = 0;
	char** t_selectedAspectRatioResolutionLabels = get_resolution_labels(g_resolutionSettings,
		g_appliedGameSettings.videoSettings.selectedAspectRatio, &t_numLabels);
	ChangeScrollSelectorOptions(g_settingsLayout.videoTab.resolutionSelector.element.scrollSelector,
		t_selectedAspectRatioResolutionLabels, t_numLabels);

	return 0;
}

void initialize(GameState *gs) {
	/***** Game Data *****/
	gs->balance = DEFAULT_BALANCE;
	gs->paused = 0;

	/***** UI *****/
	// Init UI
	height = GetScreenHeight();
	width = GetScreenWidth();
	gs->ui.dropButton = CreateButton("Drop", ((width/3)*2)+10, 10, (width/3)-20, 30);
	gs->ui.dropButton.callback = &dropButtonCallback;
	gs->ui.balanceDisplay = CreateNumberLabel("Balance", &gs->balance, 10, 10, (width/3)-20, 30);

	/*********************
	 * SETTINGS OVERHAUL *
	 *********************/
	g_settingsLayout.videoTab.aspectSelector.element.scrollSelector->OnChange = &aspectOnChange;
	g_settingsLayout.videoTab.resolutionSelector.element.scrollSelector->OnChange = &resolutionOnChange;
	g_settingsLayout.applyButton.callback = &pauseApplyButtonCallback;
	g_settingsLayout.closeButton.callback = &pauseCloseButtonCallback;

	/***** Game Objects *****/
	// Pegs
	int num_in_row = 1;
	int pyramid_start_x = width/2;
	const int pyramid_offset_x = 75;
	const int pyramid_start_y = height/5;
	const int pyramid_offset_y = 50;
	for (int i = 0; i < DEFAULT_PEGS; i++) {
		int row_position = (i+1) - (SumTo(num_in_row) - num_in_row);
		gs->objects.pegs[i][0] = (pyramid_offset_x * (row_position-1)) + pyramid_start_x;
		gs->objects.pegs[i][1] = (pyramid_offset_y * num_in_row) + pyramid_start_y;
		if ((i+1) == SumTo(num_in_row)) {
			num_in_row++;
			pyramid_start_x -= pyramid_offset_x/2;
		}
	}

	// Generate zones
	zone_width = (width/5);
	gs->zone_width = zone_width;
	for (int i = 0; i < 5; i++) {
		gs->objects.zone_animation_state[i] = 0;
		gs->objects.zone_location[i] = zone_width * i;
	}
}

enum Screen GameScreen(Font defaultFont) {
	// Test diagnostic info
	printf("Running test stuff!\n");
	// settingslayout_test();
	initialize_appliedgamesettings(&g_appliedGameSettings);
	initialize_settingslayout(&g_settingsLayout);
	initialize_resolutionsettings(&g_resolutionSettings);
	int t_numLabels = 0;
	char** t_selectedAspectRatioResolutionLabels = get_resolution_labels(g_resolutionSettings,
		g_appliedGameSettings.videoSettings.selectedAspectRatio, &t_numLabels);
	ChangeScrollSelectorOptions(g_settingsLayout.videoTab.resolutionSelector.element.scrollSelector,
		t_selectedAspectRatioResolutionLabels, t_numLabels);

	// Init game screen
	enum Screen next_screen = CLOSE_GAME;
	font = defaultFont;
	unsigned int frame_count = 0;

	// Initialize game state
	GameState gameState = {0};
	g_gameState = &gameState;
	initialize(&gameState);

	// Generate balls
	PBall *balls_head = (PBall*) malloc(sizeof(PBall)); // fix this later
	balls_head->x = -1;
	balls_head->y = -1;
	balls_head->dx = 0;
	balls_head->dy = 0;
	balls_head->radius = 0;
	balls_head->next = NULL;
	balls_head->prev = NULL;
	PBall *balls_tail = balls_head;
	PBall *balls_curr = balls_head;

	// Set up balls pointers in game state
	gameState.balls_head = &balls_head;
	gameState.balls_curr = &balls_curr;
	gameState.balls_tail = &balls_tail;

	// Run game screen
	while (!WindowShouldClose()) {
		// ********** Update **********
		// Update each ball
		balls_curr = balls_head->next;
		while (balls_curr != NULL) {
			balls_curr->x += balls_curr->dx;
			balls_curr->y += balls_curr->dy;

			// Check if a ball goes off screen
			if (balls_curr->y < 0 || balls_curr->y > height) {
				remove_ball(&balls_curr, &balls_tail);
				continue;
			}

			if ((balls_curr->x - balls_curr->radius) < 0) {
				balls_curr->dx = abs(balls_curr->dx);
			} else if ((balls_curr->x + balls_curr->radius) > width) {
				balls_curr->dx = -abs(balls_curr->dx);
			}

			// Check zone collision
			if (balls_curr->y > height-25 && balls_curr->y < height) {
				int raw_zone = ((balls_curr->x/gameState.zone_width)+1);
				int zone = abs(raw_zone - 3);
				remove_ball(&balls_curr, &balls_tail);
				gameState.balance += zone;
				gameState.objects.zone_animation_state[raw_zone-1] = 1; // start the zone animation
				printf("Ball Collided with Zone %d. Paying out %d.\n", raw_zone, zone);
				continue;
			}

			// Check peg collision
			for (int i = 0; i < DEFAULT_PEGS; i++) {
				int adj = balls_curr->x - gameState.objects.pegs[i][0];
				int opp = balls_curr->y - gameState.objects.pegs[i][1];
				int hyp = sqrt((adj*adj) + (opp*opp));
				if (hyp < balls_curr->radius + PEG_RADIUS) { // collision
					if (adj != 0) {
						double theta = atan(opp/adj);
						printf("Ball collided with peg %d\nTheta is %f\n", i, theta);
						int dx = cos(theta) * 9;
						int dy = sin(theta) * 9;
						if (adj < 0) {
							balls_curr->dx = -dx;
						} else {
							balls_curr->dx = dx;
						}
						balls_curr->dy = -dy;
					} else {
						balls_curr->dy = -8;
						balls_curr->dx = -4;
					}
				}
			}

			// gravity
			if (balls_curr->dy < 9 && frame_count % 4 == 0) {
				balls_curr->dy += 1;
			}

			balls_curr = balls_curr->next;
		}

		// Update zone animations
		for (int i = 0; i < 5; i++) {
			if (gameState.objects.zone_animation_state[i] <= 0) {
				continue;
			}

			if (gameState.objects.zone_animation_state[i] > ZONE_ANIMATION_FRAMES) {
				gameState.objects.zone_animation_state[i] = 0;
			} else {
				gameState.objects.zone_animation_state[i]++;
			}
		}

		// ********** Render **********
		BeginDrawing();
		ClearBackground(DARKBLUE);

		// Draw pegs
		for (int i = 0; i < DEFAULT_PEGS; i++) {
			DrawCircle(gameState.objects.pegs[i][0], gameState.objects.pegs[i][1], PEG_RADIUS, WHITE);
		}

		// Draw zones
		for (int i = 0; i < 5; i++) {
			int zone = abs(i - 2);
			if (gameState.objects.zone_animation_state[i]) {
				draw_zone(gameState.objects.zone_location[i], ZONE_ANIMATION_OFFSET, zone);
			} else {
				draw_zone(gameState.objects.zone_location[i], 0, zone);
			}
		}

		// Draw balls
		balls_curr = balls_head->next;
		while (balls_curr != NULL) {
			DrawCircle(balls_curr->x, balls_curr->y, balls_curr->radius, BLUE);
			balls_curr = balls_curr->next;
		}

		// Draw UI
		DrawButton(gameState.ui.dropButton, font);
		DrawNumberLabel(gameState.ui.balanceDisplay, font);

		// Draw Pause Menu
		if (gameState.paused) {
			// Draw back drop
			DrawRectangle((width-g_settingsLayout.backdropLayout.width)/2,
				((height-g_settingsLayout.backdropLayout.height)/2) + 2,
				g_settingsLayout.backdropLayout.width, g_settingsLayout.backdropLayout.height,
				BLACK);
			DrawRectangle((width-g_settingsLayout.backdropLayout.width)/2,
				(height-g_settingsLayout.backdropLayout.height)/2,
				g_settingsLayout.backdropLayout.width, g_settingsLayout.backdropLayout.height,
				BLUE);

			// Settings Overhaul Stuff
			DrawButton(g_settingsLayout.applyButton, font);
			DrawButton(g_settingsLayout.closeButton, font);

			if (g_settingsLayout.activeTab == 0) { // Video
				VideoTabLayout *t_videoTab = &g_settingsLayout.videoTab;
				for (int i = 0; i < t_videoTab->num_elements; i++) {
					DrawUIElement(*t_videoTab->layout[i], font);
				}
			} else if (g_settingsLayout.activeTab == 1) { // Audio
				AudioTabLayout *t_audioTab = &g_settingsLayout.audioTab;
				for (int i = 0; i < t_audioTab->num_elements; i++) {
					DrawUIElement(*t_audioTab->layout[i], font);
				}
			} else { // Game
				GameTabLayout *t_gameTab = &g_settingsLayout.gameTab;
				for (int i = 0; i < t_gameTab->num_elements; i++) {
					DrawUIElement(*t_gameTab->layout[i], font);
				}
			}
		}

		EndDrawing();

		// ********** Input **********
		if (!gameState.paused) {
			if (IsKeyPressed(KEY_ESCAPE)) {
				gameState.paused = 1;
				continue;
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("[Mouse Event] Mouse Pressed at (%d, %d)\n", mouseX, mouseY);

				ButtonPressed(&gameState.ui.dropButton, mouseX, mouseY);
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("[Mouse Event] Mouse Released at (%d, %d)\n", mouseX, mouseY);

				ButtonReleased(&gameState.ui.dropButton, mouseX, mouseY);
			}
		} else { // Pause Menu
			if (IsKeyPressed(KEY_ESCAPE)) {
				gameState.paused = 0;
				continue;
			}

			if (IsKeyPressed(KEY_E)) {
				g_settingsLayout.activeTab += 1;
			}

			if (IsKeyPressed(KEY_Q)) {
				g_settingsLayout.activeTab -= 1;
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("[Mouse Event] Mouse Pressed at (%d, %d)\n", mouseX, mouseY);

				if (g_settingsLayout.activeTab == 0) { // Video
					VideoTabLayout *t_videoTab = &g_settingsLayout.videoTab;
					for (int i = 0; i < t_videoTab->num_elements; i++) {
						CheckUIElementPressed(*t_videoTab->layout[i], mouseX, mouseY);
					}
				} else if (g_settingsLayout.activeTab == 1) { // Audio
					AudioTabLayout *t_audioTab = &g_settingsLayout.audioTab;
					for (int i = 0; i < t_audioTab->num_elements; i++) {
						CheckUIElementPressed(*t_audioTab->layout[i], mouseX, mouseY);
					}
				} else { // Game
					GameTabLayout *t_gameTab = &g_settingsLayout.gameTab;
					for (int i = 0; i < t_gameTab->num_elements; i++) {
						CheckUIElementPressed(*t_gameTab->layout[i], mouseX, mouseY);
					}
				}

				ButtonPressed(&g_settingsLayout.applyButton, mouseX, mouseY);
				ButtonPressed(&g_settingsLayout.closeButton, mouseX, mouseY);
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("[Mouse Event] Mouse Released at (%d, %d)\n", mouseX, mouseY);

				if (g_settingsLayout.activeTab == 0) { // Video
					VideoTabLayout *t_videoTab = &g_settingsLayout.videoTab;
					for (int i = 0; i < t_videoTab->num_elements; i++) {
						CheckUIElementReleased(*t_videoTab->layout[i], mouseX, mouseY);
					}
				} else if (g_settingsLayout.activeTab == 1) { // Audio
					AudioTabLayout *t_audioTab = &g_settingsLayout.audioTab;
					for (int i = 0; i < t_audioTab->num_elements; i++) {
						CheckUIElementReleased(*t_audioTab->layout[i], mouseX, mouseY);
					}
				} else { // Game
					GameTabLayout *t_gameTab = &g_settingsLayout.gameTab;
					for (int i = 0; i < t_gameTab->num_elements; i++) {
						CheckUIElementReleased(*t_gameTab->layout[i], mouseX, mouseY);
					}
				}

				ButtonReleased(&g_settingsLayout.applyButton, mouseX, mouseY);
				ButtonReleased(&g_settingsLayout.closeButton, mouseX, mouseY);
			}
		}

		frame_count++;
	}

	return next_screen;
}