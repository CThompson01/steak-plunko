#include <stdio.h>
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