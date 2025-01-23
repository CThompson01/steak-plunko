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
int CheckButtonPress(UIButton button, int mx, int my);

typedef struct uinumberlabel UINumberLabel;
struct uinumberlabel {
    int x;
    int y;
    int width;
    int height;
    int *value;
    char label[50];
};
void DrawNumberLabel(UINumberLabel label, Font font);

enum UIType { UIT_BUTTON, UIT_NUMLABEL };
typedef struct uielement UIElement;
struct uielement {
    enum UIType type;
    char key[25];
    union {
        UIButton button;
        UINumberLabel numLabel;
    }element;
};

void DrawUIElement(UIElement element, Font font);
int GetIndexOfKey(UIElement *elements, int elements_len, char *key);
UIElement CreateButtonElement(char key[], char label[], int x, int y, int width, int height);
UIElement CreateNumberLabelElement(char key[], char label[], int *value, int x, int y, int width, int height);

#endif