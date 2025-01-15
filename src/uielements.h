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

typedef struct scrollselector ScrollSelector;
struct scrollselector { // potential to move this to global later
    int x;
    int y;
    int selected;
    char label[50];
    int num_options;
    char *options[];
};

ScrollSelector* CreateScrollSelector(char label[],
    char *list_of_options[], int num_options, int x, int y);
void DrawScrollSelector(ScrollSelector *s, Font font);

#endif