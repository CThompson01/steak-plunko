#include "../global.h"

#define PADDING 20
#define SUBSCREENS 3

// TODO: PICK ONE NAMING CONVENTION AND STICK TO IT PLEASE

// probably redo with a struct eventually
enum OptionsSubscreen {
	VIDEO_SETTINGS,
	AUDIO_SETTINGS,
	GAME_SETTINGS
};

struct Resolution {
	int width;
	int height;
};

struct Dropdown { // potential to move this to global later
	int x;
	int y;
	int size;
	int selected;
	int is_expanded;
	char label[50];
	int num_options;
	char *options[];
};

struct Dropdown* createDropdown(char label[], char *list_of_options[], int num_options, int x, int y, int size) {
	struct Dropdown *dropdown = malloc(sizeof(struct Dropdown) + (num_options*sizeof(*dropdown->options)));
	
	dropdown->num_options = num_options;
	dropdown->selected = 0;
	dropdown->is_expanded = 0;
	dropdown->x = x;
	dropdown->y = y;
	dropdown->size = size;
	strcpy(dropdown->label, label);
	
	for (int i = 0; i < num_options; i++) {
		dropdown->options[i] = list_of_options[i];
	}

	return dropdown;
}

void renderDropdown(struct Dropdown *d, Font font, Color color) {
	DrawTextEx(font, d->label, (Vector2) {d->x, d->y}, d->size, 0, color);
	int offset = MeasureTextEx(font, d->label, d->size, 0).x + PADDING;
	if (!d->is_expanded) {
		DrawTextEx(font, d->options[d->selected], (Vector2) {offset + d->x, d->y}, d->size, 0, WHITE);
	} else {
		for (int i = 0; i < d->num_options; i++) {
			if (d->selected == i) {
				DrawTextEx(font, d->options[i], (Vector2) {offset + d->x, d->y + (d->size*i)}, 
					d->size, 0, YELLOW);
			} else {
				DrawTextEx(font, d->options[i], (Vector2) {offset + d->x, d->y + (d->size*i)}, 
					d->size, 0, WHITE);
			}
		}
	}
}

struct Checkbox {
	int x;
	int y;
	int size;
	int is_checked;
	char label[50];
};

struct Checkbox createCheckbox(char label[], int is_checked, int x, int y, int size) {
	struct Checkbox checkbox = {x, y, size, is_checked};
	strcpy(checkbox.label, label);
	return checkbox;
}

void renderCheckbox(struct Checkbox c, Font font, Color color) {
	DrawTextEx(font, c.label, (Vector2) {c.x, c.y}, c.size, 0, color);
	int offset = MeasureTextEx(font, c.label, c.size, 0).x + PADDING;
	DrawRectangle(c.x + offset, c.y, c.size, c.size, WHITE);
	if (c.is_checked) {
		DrawTextEx(font, "X", (Vector2) {c.x + offset + (c.size/4), c.y}, c.size, 0, RED);
	}
}

int options_render() {
	return 0;
}

int options_update() {
	return 0;
}

#define render options_render
#define update options_update
void onVideoApply(struct Resolution applyRes, int shouldFullscreen) {
	SetWindowSize(applyRes.width, applyRes.height);
	if (shouldFullscreen != IsWindowFullscreen()) {
		ToggleFullscreen();
	}
	// for unknown reasons, without applying resolution changes twice
	// the resolution just sometimes wont get properly set
	// SetWindowSize(applyRes.width, applyRes.height);
}

enum Screen options_screen(Font defaultFont) {
	// Init options screen
	enum Screen next_screen = CLOSE_GAME;

	// Subscreen and settings tracking stuff
	enum OptionsSubscreen subscreen = VIDEO_SETTINGS;
	char* subscreens_labels[] = {"Video", "Audio", "Game"};
	int selectedSetting = 0; // TODO: do this in a much much better future proofed way
	int settingsOnSubscreen[] = {2, 0, 0};

	// Video Settings
	struct Resolution resolutions[] = {{854, 480}, {1280, 720}, {1920, 1080}, {2560, 1440}};
	struct Resolution selectedResolution = resolutions[2];
	int fullscreened = 0;
	struct Dropdown *resolutionDropdown = createDropdown("Resolution", (char*[]) {"854x480", "1280x720", "1920x1080"}, 3, 40, 200, 40);
	struct Checkbox fullscreenCheckbox = createCheckbox("Fullscreen", fullscreened, 40, 250, 40);


	// Rendering math bs
	int height = GetScreenHeight();
	int width = GetScreenWidth();
	int bw = (width/3);
	int tw;

	// Run options screen
	while (!WindowShouldClose()) {
		// ********** Render **********
		BeginDrawing();
		ClearBackground(BLACK);

		// Draw header
		DrawTextEx(defaultFont, "Settings", (Vector2) {20, 20}, height/12, 0, WHITE);
		
		// Draw Subscreen options
		for (int i = 0; i < 3; i++) { // fix hardcoding issue later but its fine for now
			tw = MeasureTextEx(defaultFont, subscreens_labels[i], height/16, 0).x/2;
			int xPos = ((bw-tw)/2) + (bw*i);
			if (subscreen == i && !selectedSetting) {
				DrawTextEx(defaultFont, subscreens_labels[i], (Vector2) {xPos, 40 + height/12}, height/16, 0, BLUE);
			} else {
				DrawTextEx(defaultFont, subscreens_labels[i], (Vector2) {xPos, 40 + height/12}, height/16, 0, WHITE);
			}
		}

		// Render different subscreens here
		if (subscreen == VIDEO_SETTINGS) { // Render Video settings
			renderDropdown(resolutionDropdown, defaultFont, (selectedSetting == 1 ? BLUE : WHITE));
			renderCheckbox(fullscreenCheckbox, defaultFont, (selectedSetting == 2 ? BLUE : WHITE));
		} else if (subscreen == GAME_SETTINGS) { // Render Game settings
			DrawTextEx(defaultFont, "GAME SETTINGS", (Vector2) {20, 80 + height/12 + height/16}, height/16, 0, WHITE);
		} else if (subscreen == AUDIO_SETTINGS) { // Render Audio Settings
			DrawTextEx(defaultFont, "AUDIO SETTINGS", (Vector2) {20, 80 + height/12 + height/16}, height/16, 0, WHITE);
		} else {
			next_screen = CLOSE_GAME;
			break;
		}

		EndDrawing();
		
		// ********** Update **********
		if (IsKeyPressed(KEY_ESCAPE)) {
			next_screen = CLOSE_GAME;
			break;
		}

		if (IsKeyPressed(KEY_LEFT) && 
				selectedSetting == 0 && 
				subscreen > 0) {
			subscreen -= 1;	
		}

		if (IsKeyPressed(KEY_RIGHT) && 
				selectedSetting == 0 && 
				subscreen < SUBSCREENS-1) {
			subscreen += 1;	
		}

		// This is bad, fix it later
		int doingSomething = 0;
		if (subscreen == VIDEO_SETTINGS) {
			if (IsKeyPressed(KEY_ENTER)) {
				if (selectedSetting == 1) {
					if (resolutionDropdown->is_expanded) {
						resolutionDropdown->is_expanded = 0;
						selectedResolution = resolutions[resolutionDropdown->selected];
						onVideoApply(selectedResolution, 0);
						height = GetScreenHeight();
						width = GetScreenWidth();
						bw = (width/3);
					} else {
						resolutionDropdown->is_expanded = 1;
					}
				} else if (selectedSetting == 2) {
					fullscreened = !fullscreened;
					fullscreenCheckbox.is_checked = fullscreened;
				}
			}

			if (IsKeyPressed(KEY_UP)) {
				if (resolutionDropdown->is_expanded && resolutionDropdown->selected > 0 ) {
					doingSomething = 1;
					resolutionDropdown->selected -= 1;
				}
			}

			if (IsKeyPressed(KEY_DOWN)) {
				if (resolutionDropdown->is_expanded && resolutionDropdown->selected < resolutionDropdown->num_options-1) {
					doingSomething = 1;
					resolutionDropdown->selected += 1;
				}
			}
		}

		if (!doingSomething) {
			if (IsKeyPressed(KEY_UP) && selectedSetting > 0) {
				selectedSetting -= 1;
			}

			if (IsKeyPressed(KEY_DOWN) && selectedSetting < settingsOnSubscreen[subscreen]) {
				selectedSetting += 1;
			}
		}
	}
	
	return next_screen;
}
#undef render
#undef update