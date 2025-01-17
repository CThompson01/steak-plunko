#ifndef UIELEMENTS_H
#define UIELEMENTS_H

typedef struct uibutton UIButton;
struct uibutton {
    int x;
    int y;
    int width;
    int height;
    char label[50];
};

void DrawButton(UIButton button, Font font);
void DrawLabelWithValue(UIButton button, Font font, int value);
int CheckButtonPress(int mx, int my, UIButton button);

enum UIType { BUTTON };
typedef struct uielement UIElement;
struct uielement {
    enum UIType type;
    union {
        UIButton button;
    };
};

void DrawUIElement(UIElement element, Font font);

#endif