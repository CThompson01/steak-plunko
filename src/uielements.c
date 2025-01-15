#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "uielements.h"

void DrawButton(UIButton button, Font font) {
    DrawRectangle(button.x, button.y, button.width, button.height, GRAY);
    int textOffset = MeasureTextEx(font, button.label, 28, 0).x/2;
    DrawTextEx(font, button.label, (Vector2) {(button.x+(button.width/2))-textOffset, button.y},
        28, 0, WHITE);
}

void DrawDropShadowButton(UIButton button, Font font, int offset) {
    if (!button.pressed) {
        DrawRectangle(button.x, button.y + offset, button.width, button.height, BLACK);
        DrawRectangle(button.x, button.y, button.width, button.height, GRAY);
        int textOffset = MeasureTextEx(font, button.label, 28, 0).x/2;
        DrawTextEx(font, button.label, (Vector2) {(button.x+(button.width/2))-textOffset, button.y},
            28, 0, WHITE);
    } else {
        DrawRectangle(button.x, button.y+offset, button.width, button.height, GRAY);
        int textOffset = MeasureTextEx(font, button.label, 28, 0).x/2;
        DrawTextEx(font, button.label, (Vector2) {(button.x+(button.width/2))-textOffset, button.y + offset},
            28, 0, WHITE);
    }
}

void DrawLabelWithValue(UIButton button, Font font, int value) {
    char label[125] = "";
    sprintf(label, "%s: %d", button.label, value);
    DrawRectangle(button.x, button.y, button.width, button.height, GRAY);
    int textOffset = MeasureTextEx(font, label, 28, 0).x/2;
    DrawTextEx(font, label, (Vector2) {(button.x+(button.width/2))-textOffset, button.y},
        28, 0, WHITE);
}

int CheckButtonPress(int mx, int my, UIButton button) {
    int xDiff = mx - button.x;
    int yDiff = my - button.y;
    return (xDiff < button.width) && (yDiff < button.height) &&
        (xDiff > 0) && (yDiff > 0);
}

ScrollSelector* CreateScrollSelector(char label[], char *list_of_options[], int num_options, int x, int y) {
    ScrollSelector *scroll_selector = malloc(sizeof(ScrollSelector) + (num_options*sizeof(*scroll_selector->options)));
    
    scroll_selector->num_options = num_options;
    scroll_selector->selected = 0;
    scroll_selector->x = x;
    scroll_selector->y = y;
    strcpy(scroll_selector->label, label);
    
    for (int i = 0; i < num_options; i++) {
        scroll_selector->options[i] = list_of_options[i];
    }

    return scroll_selector;
}

// X position is centered, Y position is top
void DrawScrollSelector(ScrollSelector *s, Font font) {
    const unsigned int FONT_HEIGHT = 20;
    const unsigned int X_PADDING = 8;
    const unsigned int Y_PADDING = 4;

    // Draw the label
    int label_offset = MeasureTextEx(font, s->label, FONT_HEIGHT, 0).x/2;
    DrawTextEx(font, s->label, (Vector2) {s->x-label_offset, s->y}, FONT_HEIGHT, 0, WHITE);

    // Draw the selected option
    int tb_y = s->y + FONT_HEIGHT + Y_PADDING;
    int tb_width = MeasureTextEx(font, s->options[s->selected], FONT_HEIGHT, 0).x + (X_PADDING*2);
    int tb_height = FONT_HEIGHT + (2 * Y_PADDING);
    DrawRectangle(s->x-(tb_width/2), tb_y,
        tb_width, tb_height + 2, BLACK); // TODO: remove double drawing
    DrawRectangle(s->x-(tb_width/2), tb_y,
        tb_width, tb_height, DARKBLUE);
    DrawTextEx(font, s->options[s->selected],
        (Vector2) {(s->x + X_PADDING) - (tb_width/2), tb_y + Y_PADDING},
        FONT_HEIGHT, 0, WHITE);

    // Draw the right button
    int tb_x_r = s->x + (tb_width/2) + (X_PADDING/2);
    DrawRectangle(tb_x_r, tb_y,
        tb_height, tb_height + 2, BLACK); // TODO: remove double drawing
    DrawRectangle(tb_x_r, tb_y,
        tb_height, tb_height, DARKBLUE);
    DrawTextEx(font, ">",
        (Vector2) {tb_x_r + X_PADDING, tb_y + Y_PADDING},
        FONT_HEIGHT, 0, WHITE);

    // Draw the left button
    int tb_x_l = s->x - ((tb_width/2) + (X_PADDING/2) + tb_height);
    DrawRectangle(tb_x_l, tb_y,
        tb_height, tb_height + 2, BLACK); // TODO: remove double drawing
    DrawRectangle(tb_x_l, tb_y,
        tb_height, tb_height, DARKBLUE);
    DrawTextEx(font, "<",
        (Vector2) {tb_x_l + X_PADDING, tb_y + Y_PADDING},
        FONT_HEIGHT, 0, WHITE);
}