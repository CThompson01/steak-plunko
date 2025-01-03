#include <math.h>
#include "../global.h"

#define DEFAULT_PEGS 6
#define PEG_RADIUS 12
#define DROP_RANGE 250

Font font;

typedef struct uibutton UIButton;
struct uibutton {
	int x;
	int y;
	int width;
	int height;
	char label[50];
};

void draw_button(UIButton button) {
	DrawRectangle(button.x, button.y, button.width, button.height, GRAY);
	int textOffset = MeasureTextEx(font, button.label, 28, 0).x/2;
	DrawTextEx(font, button.label, (Vector2) {(button.x+(button.width/2))-textOffset, button.y},
		28, 0, WHITE);
}

void draw_label_with_value(UIButton button, int value) {
	char label[125] = "";
	sprintf(label, "%s: %d", button.label, value);
	DrawRectangle(button.x, button.y, button.width, button.height, GRAY);
	int textOffset = MeasureTextEx(font, label, 28, 0).x/2;
	DrawTextEx(font, label, (Vector2) {(button.x+(button.width/2))-textOffset, button.y},
		28, 0, WHITE);
}

int check_button_press(int mx, int my, UIButton button) {
	int xDiff = mx - button.x;
	int yDiff = my - button.y;
	return (xDiff < button.width) && (yDiff < button.height) &&
		(xDiff > 0) && (yDiff > 0);
}

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
	return randomInRange(0, DROP_RANGE) + ((GetScreenWidth()-DROP_RANGE)/2);
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

enum Screen game_screen(Font defaultFont) {
	// Init game screen
	enum Screen next_screen = CLOSE_GAME;
	font = defaultFont;
	int frame_count = 0;

	// Generate static screen layout values
	int height = GetScreenHeight();
	int width = GetScreenWidth();
	UIButton dropBallButton = {((width/3)*2)+10, 10, (width/3)-20, 30, "Drop"};
	int balance = 100;
	UIButton balanceDisplay = {10, 10, (width/3)-20, 30, "Balance"};

	// Generate game objects
	// Generate pegs
	int pegs[DEFAULT_PEGS][2] = {0};
	pegs[0][0] = width/2;
	pegs[0][1] = 200;
	pegs[1][0] = (width/2)-(width/8);
	pegs[1][1] = 250;
	pegs[2][0] = (width/2)+(width/8);
	pegs[2][1] = 250;
	pegs[3][0] = width/4;
	pegs[3][1] = 300;
	pegs[4][0] = (width/4)*2;
	pegs[4][1] = 300;
	pegs[5][0] = (width/4)*3;
	pegs[5][1] = 300;

	// Generate zones
	int zone_width = (width/5);
	int zone_location[5] = {0};
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
			if (balls_curr->x < 0 || balls_curr->x > width || balls_curr->y < 0) {
				remove_ball(&balls_curr, &balls_tail);
				continue;
			}

			// Check zone collision
			if (balls_curr->y > height-25 && balls_curr->y < height) {
				int raw_zone = ((balls_curr->x/zone_width)+1);
				int zone = abs(raw_zone - 3);
				remove_ball(&balls_curr, &balls_tail);
				balance += zone;
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

		// ********** Render **********
		BeginDrawing();
		ClearBackground(BLACK);

		// Draw pegs
		for (int i = 0; i < sizeof(pegs)/sizeof(pegs[0]); i++) {
			DrawCircle(pegs[i][0], pegs[i][1], PEG_RADIUS, WHITE);
		}

		// Draw zones
		for (int i = 0; i < sizeof(zone_location)/sizeof(zone_location[0]); i++) {
			if (i == 0 || i == 4) {
				DrawRectangle(zone_location[i], height-25, zone_width, 25, GREEN);
			} else if (i == 1 || i == 3) {
				DrawRectangle(zone_location[i], height-25, zone_width, 25, ORANGE);
			} else {
				DrawRectangle(zone_location[i], height-25, zone_width, 25, RED);
			}
		}

		// Draw balls
		balls_curr = balls_head->next;
		while (balls_curr != NULL) {
			DrawCircle(balls_curr->x, balls_curr->y, balls_curr->radius, BLUE);
			balls_curr = balls_curr->next;
		}

		// Draw UI
		draw_button(dropBallButton);
		draw_label_with_value(balanceDisplay, balance);

		EndDrawing();

		// ********** Input **********
		if (IsKeyPressed(KEY_ESCAPE)) {
			next_screen = CLOSE_GAME;
			break;
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			int mouseX = GetMouseX();
			int mouseY = GetMouseY();
			printf("Mouse Down at (%d, %d)\n", mouseX, mouseY);
			if (check_button_press(mouseX, mouseY, dropBallButton) && balance > 0) {
				printf("Generating Ball\n");
				generate_ball(&balls_tail);
				balance--;
			}
		}

		frame_count++;
	}
	
	return next_screen;
}