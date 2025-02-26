#ifndef UIELEMENTS_H
#define UIELEMENTS_H

typedef struct uibutton {
    int x;
    int y;
    int width;
    int height;
    int (*callback)(void*);
    char label[50];
    int pressed;
} UIButton;

UIButton CreateButton(char label[], int x, int y, int width, int height);
void DrawButton(UIButton button, Font font);
int CheckButtonPress(UIButton button, int mx, int my);
void ButtonPressed(UIButton *button, int mx, int my, void *context);
void ButtonReleased(UIButton *button, int mx, int my, void *context);
int DefaultButtonCallback();

typedef struct uinumberlabel {
    int x;
    int y;
    int width;
    int height;
    int *value;
    char label[50];
} UINumberLabel;
UINumberLabel CreateNumberLabel(char label[], int *value, int x, int y, int width, int height);
void DrawNumberLabel(UINumberLabel label, Font font);

typedef struct scrollselector ScrollSelector;
struct scrollselector { // potential to move this to global later
    int x;
    int y;
    int selected;
    char label[50];

    int display_y; // Button location stuff
    int display_width;
    int display_height;
    int button_r_x;
    int button_l_x;

    int left_pressed; // Release button stuff
    int right_pressed;

    int (*OnChange)(char selection[], void*);
    int num_options;
    char *options[];
};

ScrollSelector* CreateScrollSelector(char label[], char *list_of_options[], int num_options, int x, int y);
void DrawScrollSelector(ScrollSelector *s, Font font);
int ScrollSelectorInput(ScrollSelector *s, int mx, int my);
void ScrollSelectorPressed(ScrollSelector *s, int mx, int my, void *context);
void ScrollSelectorReleased(ScrollSelector *s, int mx, int my, void *context);
void ChangeScrollSelectorOptions(ScrollSelector *s, char *list_of_options[], int num_options);
void updateScrollSelectorPositions(ScrollSelector *s, Font font);

enum UIType { UIT_BUTTON, UIT_NUMLABEL, UIT_SCROLLSELECTOR };
typedef struct uielement {
    enum UIType type;
    char key[25];
    union {
        UIButton button;
        UINumberLabel numLabel;
        ScrollSelector *scrollSelector;
    } element;
} UIElement;

void DrawUIElement(UIElement element, Font font);
void CheckUIElementPressed(UIElement element, int mx, int my, void *context);
void CheckUIElementReleased(UIElement element, int mx, int my, void *context);
int GetIndexOfKey(UIElement *elements, int elements_len, char *key);
void UIElementUpdatePosition(UIElement element, int new_x, int new_y);
UIElement CreateButtonElement(char key[], char label[], int x, int y, int width, int height);
UIElement CreateNumberLabelElement(char key[], char label[], int *value, int x, int y, int width, int height);
UIElement CreateScrollSelectorElement(char key[], char label[], char *list_of_options[], int num_options, int x, int y);

#endif