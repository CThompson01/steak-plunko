#ifndef UIELEMENTS_H
#define UIELEMENTS_H

typedef struct uibutton UIButton;
struct uibutton {
    int x;
    int y;
    int width;
    int height;
    char label[50];
    int pressed;
};

void DrawButton(UIButton button, Font font);
void DrawDropShadowButton(UIButton button, Font font, int offset);
void DrawLabelWithValue(UIButton button, Font font, int value);
int CheckButtonPress(int mx, int my, UIButton button);

#endif