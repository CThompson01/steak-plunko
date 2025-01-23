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

int CheckButtonPress(UIButton button, int mx, int my) {
    int xDiff = mx - button.x;
    int yDiff = my - button.y;
    return (xDiff < button.width) && (yDiff < button.height) &&
        (xDiff > 0) && (yDiff > 0);
}

void DrawNumberLabel(UINumberLabel numLabel, Font font) {
    char label[125] = "";
    sprintf(label, "%s: %d", numLabel.label, *numLabel.value);
    DrawRectangle(numLabel.x, numLabel.y, numLabel.width, numLabel.height, GRAY);
    int textOffset = MeasureTextEx(font, label, 28, 0).x/2;
    DrawTextEx(font, label, (Vector2) {(numLabel.x+(numLabel.width/2))-textOffset, numLabel.y},
        28, 0, WHITE);
}

void DrawUIElement(UIElement element, Font font) {
    switch (element.type) {
        case UIT_BUTTON:
            DrawButton(element.element.button, font);
            break;
        case UIT_NUMLABEL:
            DrawNumberLabel(element.element.numLabel, font);
            break;
        default:
            printf("UI Type not found.\n");
            break;
    }
}

int GetIndexOfKey(UIElement *elements, int elements_len, char *key) {
    for (int i = 0; i < elements_len; i++) {
        if (strcmp(elements[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

UIElement CreateButtonElement(char key[], char label[], int x, int y, int width, int height) {
    UIButton button = {x, y, width, height};
    strcpy(button.label, label);
    UIElement element = {UIT_BUTTON};
    strcpy(element.key, key);
    element.element.button = button;
    return element;
}

UIElement CreateNumberLabelElement(char *key, char *label, int *value, int x, int y, int width, int height) {
    UINumberLabel numLabel = {x, y, width, height, value};
    strcpy(numLabel.label, label);
    UIElement element = {UIT_NUMLABEL};
    strcpy(element.key, key);
    element.element.numLabel = numLabel;
    return element;
}