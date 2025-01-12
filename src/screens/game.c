#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "../uielements.h"
#include "../global.h"

#define DEFAULT_PEGS 15
#define PEG_RADIUS 12
#define DROP_RANGE 250

#define ZONE_ANIMATION_FRAMES 8
#define ZONE_ANIMATION_OFFSET 8

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

enum Screen GameScreen(Font defaultFont) {
	// Init game screen
	enum Screen next_screen = CLOSE_GAME;
	font = defaultFont;
	unsigned int frame_count = 0;
	int paused = 0;

	// Implement static screen layout values
	height = GetScreenHeight();
	width = GetScreenWidth();
	UIButton dropBallButton = {((width/3)*2)+10, 10, (width/3)-20, 30, "Drop"};
	int balance = 99;
	UIButton balanceDisplay = {10, 10, (width/3)-20, 30, "Balance"};

	// Pause Modal Buttons
	unsigned int modal_padding_x = width/8;
	unsigned int modal_padding_y = height/4;
	unsigned int modal_width = modal_padding_x * 6;
	unsigned int modal_height = modal_padding_y * 2;

	int buttonWidth = (modal_width/4)-5;
	int buttonAreaStart = modal_padding_x + (modal_width/4);
	UIButton applyButton = {buttonAreaStart, (modal_padding_y + modal_height) - (50 + 10),
		buttonWidth, 50, "Apply", 0};
	UIButton closeButton = {buttonAreaStart + buttonWidth + 5,
		(modal_padding_y + modal_height) - (50 + 10),
		buttonWidth, 50, "Close", 0};

	// Generate game objects
	// Generate pegs
	int pegs[DEFAULT_PEGS][2] = {0};
	int num_in_row = 1;
	int pyramid_start_x = width/2;
	const int pyramid_offset_x = 75;
	const int pyramid_start_y = height/5;
	const int pyramid_offset_y = 50;
	for (int i = 0; i < DEFAULT_PEGS; i++) {
		int row_position = (i+1) - (SumTo(num_in_row) - num_in_row);
		pegs[i][0] = (pyramid_offset_x * (row_position-1)) + pyramid_start_x;
		pegs[i][1] = (pyramid_offset_y * num_in_row) + pyramid_start_y;
		if ((i+1) == SumTo(num_in_row)) {
			num_in_row++;
			pyramid_start_x -= pyramid_offset_x/2;
		}
	}

	// Generate zones
	zone_width = (width/5);
	int zone_location[5] = {0};
	int zone_animation_state[5] = {0};
	for (int i = 0; i < 5; i++) {
		zone_location[i] = zone_width * i;
	}

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
				int raw_zone = ((balls_curr->x/zone_width)+1);
				int zone = abs(raw_zone - 3);
				remove_ball(&balls_curr, &balls_tail);
				balance += zone;
				zone_animation_state[raw_zone-1] = 1; // start the zone animation
				printf("Ball Collided with Zone %d. Paying out %d.\n", raw_zone, zone);
				continue;
			}

			// Check peg collision
			for (int i = 0; i < DEFAULT_PEGS; i++) {
				int adj = balls_curr->x - pegs[i][0];
				int opp = balls_curr->y - pegs[i][1];
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
			if (zone_animation_state[i] <= 0) {
				continue;
			}

			if (zone_animation_state[i] > ZONE_ANIMATION_FRAMES) {
				zone_animation_state[i] = 0;
			} else {
				zone_animation_state[i]++;
			}
		}

		// ********** Render **********
		BeginDrawing();
		ClearBackground(DARKBLUE);

		// Draw pegs
		for (int i = 0; i < sizeof(pegs)/sizeof(pegs[0]); i++) {
			DrawCircle(pegs[i][0], pegs[i][1], PEG_RADIUS, WHITE);
		}

		// Draw zones
		for (int i = 0; i < sizeof(zone_location)/sizeof(zone_location[0]); i++) {
			int zone = abs(i - 2);
			if (zone_animation_state[i]) {
				draw_zone(zone_location[i], ZONE_ANIMATION_OFFSET, zone);
			} else {
				draw_zone(zone_location[i], 0, zone);
			}
		}

		// Draw balls
		balls_curr = balls_head->next;
		while (balls_curr != NULL) {
			DrawCircle(balls_curr->x, balls_curr->y, balls_curr->radius, BLUE);
			balls_curr = balls_curr->next;
		}

		// Draw UI
		DrawButton(dropBallButton, font);
		DrawLabelWithValue(balanceDisplay, font, balance);

		// Draw Pause Menu
		if (paused) {
			// Draw back drop
			DrawRectangle(modal_padding_x, modal_padding_y+2,
				width-(modal_padding_x*2), height-(modal_padding_y*2),
				BLACK);
			DrawRectangle(modal_padding_x, modal_padding_y,
				width-(modal_padding_x*2), height-(modal_padding_y*2),
				BLUE);

			DrawDropShadowButton(applyButton, font, 2);
			DrawDropShadowButton(closeButton, font, 2);
		}

		EndDrawing();

		// ********** Input **********
		if (!paused) {
			if (IsKeyPressed(KEY_ESCAPE)) {
				paused = 1;
				continue;
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("Mouse Down at (%d, %d)\n", mouseX, mouseY);
				if (CheckButtonPress(mouseX, mouseY, dropBallButton) && balance > 0) {
					printf("Generating Ball\n");
					generate_ball(&balls_tail);
					balance--;
				}
			}
		} else {
			if (IsKeyPressed(KEY_ESCAPE)) {
				paused = 0;
				continue;
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("Mouse Pressed at (%d, %d)\n", mouseX, mouseY);

				if (CheckButtonPress(mouseX, mouseY, applyButton)) {
					applyButton.pressed = 1;
					continue;
				} else if (CheckButtonPress(mouseX, mouseY, closeButton)) {
					closeButton.pressed = 1;
					continue;
				}
			}

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				int mouseX = GetMouseX();
				int mouseY = GetMouseY();
				printf("Mouse Released at (%d, %d)\n", mouseX, mouseY);

				if (CheckButtonPress(mouseX, mouseY, closeButton) && closeButton.pressed) {
					paused = 0;
				}

				// Unpress all buttons (TODO: find a better way to do this)
				applyButton.pressed = 0;
				closeButton.pressed = 0;
			}
		}
		

		frame_count++;
	}
	
	return next_screen;
}