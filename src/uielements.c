#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "uielements.h"

/***** Buttons *****/
UIButton CreateButton(char label[], int x, int y, int width, int height) {
    UIButton button = {x, y, width, height, &DefaultButtonCallback};
    strcpy(button.label, label);
    button.pressed = 0;
    return button;
}

void initialize_uibutton(UIButton *button, char label[], int x, int y, int width, int height) {
    *button = (UIButton) {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .pressed = 0,
        .callback = &DefaultButtonCallback
    };
    strcpy(button->label, label);
}

void DrawButton(UIButton button, Font font) {
    const int offset = 2;
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

int CheckButtonPress(UIButton button, int mx, int my) {
    int xDiff = mx - button.x;
    int yDiff = my - button.y;
    return (xDiff < button.width) && (yDiff < button.height) &&
        (xDiff > 0) && (yDiff > 0);
}

void ButtonPressed(UIButton *button, int mx, int my) {
    if (CheckButtonPress(*button, mx, my)) {
        printf("[Button Event] %s pressed\n", button->label);
        button->pressed = 1;
    }
}

void ButtonReleased(UIButton *button, int mx, int my) {
    if (CheckButtonPress(*button, mx, my) && button->pressed) {
        printf("[Button Event] %s released\n", button->label);
        button->pressed = 0;
        button->callback();
    } else {
        button->pressed = 0;
    }
}

int DefaultButtonCallback() {
    printf("Button has been pressed.\n");
    return 0;
}

/***** Number Label *****/
UINumberLabel CreateNumberLabel(char label[], int *value, int x, int y, int width, int height) {
    UINumberLabel numLabel = {x, y, width, height, value};
    strcpy(numLabel.label, label);
    return numLabel;
}

void DrawNumberLabel(UINumberLabel numLabel, Font font) {
    char label[125] = "";
    sprintf(label, "%s: %d", numLabel.label, *numLabel.value);
    DrawRectangle(numLabel.x, numLabel.y, numLabel.width, numLabel.height, GRAY);
    int textOffset = MeasureTextEx(font, label, 28, 0).x/2;
    DrawTextEx(font, label, (Vector2) {(numLabel.x+(numLabel.width/2))-textOffset, numLabel.y},
        28, 0, WHITE);
}

/***** Scroll Selector *****/
ScrollSelector* CreateScrollSelector(char label[], char *list_of_options[], int num_options, int x, int y) {
    ScrollSelector *scroll_selector = malloc(sizeof(ScrollSelector) + (num_options*sizeof(char*)));
    
    scroll_selector->num_options = num_options;
    scroll_selector->selected = 0;
    scroll_selector->x = x;
    scroll_selector->y = y;
    strcpy(scroll_selector->label, label);
    
    for (int i = 0; i < num_options; i++) {
        scroll_selector->options[i] = list_of_options[i];
    }

    // Default values
    scroll_selector->display_y = -1;
    scroll_selector->display_width = -1;
    scroll_selector->display_height = -1;
    scroll_selector->button_r_x = -1;
    scroll_selector->button_l_x = -1;
    scroll_selector->left_pressed = 0;
    scroll_selector->right_pressed = 0;
    scroll_selector->OnChange = NULL;

    return scroll_selector;
}

// X position is centered, Y position is top
void DrawScrollSelector(ScrollSelector *s, Font font) {
    if (s->button_r_x == -1) {
        printf("Updating scroll selector.\n");
        updateScrollSelectorPositions(s, font);
    }

    const unsigned int FONT_HEIGHT = 20;
    const unsigned int X_PADDING = 8;
    const unsigned int Y_PADDING = 4;

    // Draw the label
    int label_offset = MeasureTextEx(font, s->label, FONT_HEIGHT, 0).x/2;
    DrawTextEx(font, s->label, (Vector2) {s->x-label_offset, s->y}, FONT_HEIGHT, 0, WHITE);

    // Draw the selected option
    DrawRectangle(s->x-(s->display_width/2), s->display_y,
        s->display_width, s->display_height + 2, BLACK); // TODO: remove double drawing
    DrawRectangle(s->x-(s->display_width/2), s->display_y,
        s->display_width, s->display_height, DARKBLUE);
    DrawTextEx(font, s->options[s->selected],
        (Vector2) {(s->x + X_PADDING) - (s->display_width/2), s->display_y + Y_PADDING},
        FONT_HEIGHT, 0, WHITE);

    // Draw the left button
    if (s->left_pressed) {
        DrawRectangle(s->button_l_x, s->display_y + 2,
            s->display_height, s->display_height, DARKBLUE);
        DrawTextEx(font, "<",
            (Vector2) {s->button_l_x + X_PADDING, s->display_y + Y_PADDING + 2},
            FONT_HEIGHT, 0, WHITE);
    } else {
        DrawRectangle(s->button_l_x, s->display_y,
            s->display_height, s->display_height + 2, BLACK); // TODO: remove double drawing
        DrawRectangle(s->button_l_x, s->display_y,
            s->display_height, s->display_height, DARKBLUE);
        DrawTextEx(font, "<",
            (Vector2) {s->button_l_x + X_PADDING, s->display_y + Y_PADDING},
            FONT_HEIGHT, 0, WHITE);
    }

    // Draw the right button
    if (s->right_pressed) {
        DrawRectangle(s->button_r_x, s->display_y + 2,
            s->display_height, s->display_height, DARKBLUE);
        DrawTextEx(font, ">",
            (Vector2) {s->button_r_x + X_PADDING, s->display_y + Y_PADDING + 2},
            FONT_HEIGHT, 0, WHITE);
    } else {
        DrawRectangle(s->button_r_x, s->display_y,
            s->display_height, s->display_height + 2, BLACK); // TODO: remove double drawing
        DrawRectangle(s->button_r_x, s->display_y,
            s->display_height, s->display_height, DARKBLUE);
        DrawTextEx(font, ">",
            (Vector2) {s->button_r_x + X_PADDING, s->display_y + Y_PADDING},
            FONT_HEIGHT, 0, WHITE);
    }
}

// TODO: remove dev comments
// Functions similar to strcmp
// Returns 0 if neither are receiving input
// Returns greater than zero if right is receiving input
// Returns less than zero if left is receiving input
int ScrollSelectorInput(ScrollSelector *s, int mx, int my) {
    if (s->button_r_x == -1) {
        printf("Update scroll selector before checking input.\n");
        return 0;
    }

    // readability
    int bx_l = s->button_l_x;
    int by_l = s->display_y;
    int bx_r = s->button_r_x;
    int by_r = s->display_y;
    int b_wh = s->display_height; // combined width and height

    // check left button
    int xDiff_l = mx - bx_l;
    int yDiff_l = my - by_l;
    if ((xDiff_l < b_wh) && (yDiff_l < b_wh) &&
        (xDiff_l > 0) && (yDiff_l > 0)) {
        return -1;
    }

    // check right button
    int xDiff_r = mx - bx_r;
    int yDiff_r = my - by_r;
    if ((xDiff_r < b_wh) && (yDiff_r < b_wh) &&
        (xDiff_r > 0) && (yDiff_r > 0)) {
        return 1;
    }

    return 0;
}

void ScrollSelectorPressed(ScrollSelector *s, int mx, int my) {
    int scroll_input = ScrollSelectorInput(s, mx, my);

    if (scroll_input < 0) { // check left button
        s->left_pressed = 1;
    } else if (scroll_input > 0) { // check right button
        s->right_pressed = 1;
    }
}

void ScrollSelectorReleased(ScrollSelector *s, int mx, int my) {
    int scroll_input = ScrollSelectorInput(s, mx, my);
    int selection_changed = 0; // TODO make more efficient

    if (scroll_input < 0 && s->left_pressed) { // check left button
        s->selected--;
        if (s->selected < 0) {
            s->selected = s->num_options-1;
        }
        s->button_r_x = -1; // make sure positions are updated
        selection_changed = 1;
    } else if (scroll_input > 0 && s->right_pressed) { // check right button
        s->selected++;
        if (s->selected > s->num_options-1) {
            s->selected = 0;
        }
        s->button_r_x = -1; // make sure positions are updated
        selection_changed = 1;
    }

    s->left_pressed = 0;
    s->right_pressed = 0;

    if (s->OnChange != NULL && selection_changed) {
        s->OnChange(s->options[s->selected]);
    }
}

void ChangeScrollSelectorOptions(ScrollSelector *s, char *list_of_options[], int num_options) {
    // Resize the Scroll Selector memory to fit the new number of options
    s = realloc(s, sizeof(ScrollSelector) + (num_options*sizeof(char*)));

    s->num_options = num_options;
    s->button_r_x = -1;
    s->selected = 0;

    for (int i = 0; i < num_options; i++) {
        s->options[i] = list_of_options[i];
    }
}

void updateScrollSelectorPositions(ScrollSelector *s, Font font) {
    const unsigned int FONT_HEIGHT = 20;
    const unsigned int X_PADDING = 8;
    const unsigned int Y_PADDING = 4;

    int tb_y = s->y + FONT_HEIGHT + Y_PADDING;
    int tb_width = MeasureTextEx(font, s->options[s->selected], FONT_HEIGHT, 0).x + (X_PADDING*2);
    int tb_height = FONT_HEIGHT + (2 * Y_PADDING);
    int tb_x_r = s->x + (tb_width/2) + (X_PADDING/2);
    int tb_x_l = s->x - ((tb_width/2) + (X_PADDING/2) + tb_height);

    s->display_y = tb_y;
    s->display_width = tb_width;
    s->display_height = tb_height;
    s->button_r_x = tb_x_r;
    s->button_l_x = tb_x_l;
}

/***** UI ELEMENT *****/
void DrawUIElement(UIElement element, Font font) {
    switch (element.type) {
        case UIT_BUTTON:
            DrawButton(element.element.button, font);
            break;
        case UIT_NUMLABEL:
            DrawNumberLabel(element.element.numLabel, font);
            break;
        case UIT_SCROLLSELECTOR:
            DrawScrollSelector(element.element.scrollSelector, font);
            break;
        default:
            printf("UI Type not found.\n");
            break;
    }
}

void CheckUIElementPressed(UIElement element, int mx, int my) {
    switch (element.type) {
        case UIT_BUTTON:
            ButtonPressed(&element.element.button, mx, my);
            break;
        case UIT_SCROLLSELECTOR:
            ScrollSelectorPressed(element.element.scrollSelector, mx, my);
            break;
        default:
            printf("UI Type not an input.\n");
            break;
    }
}

void CheckUIElementReleased(UIElement element, int mx, int my) {
    switch (element.type) {
        case UIT_BUTTON:
            ButtonReleased(&element.element.button, mx, my);
            break;
        case UIT_SCROLLSELECTOR:
            ScrollSelectorReleased(element.element.scrollSelector, mx, my);
            break;
        default:
            printf("UI Type not an input.\n");
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

void UIElementUpdatePosition(UIElement element, int new_x, int new_y) {
    switch (element.type) {
        case UIT_BUTTON:
            element.element.button.x = new_x;
            element.element.button.y = new_y;
            break;
        case UIT_NUMLABEL:
            element.element.numLabel.x = new_x;
            element.element.numLabel.y = new_y;
            break;
        case UIT_SCROLLSELECTOR:
            element.element.scrollSelector->button_r_x = -1;
            element.element.scrollSelector->x = new_x;
            element.element.scrollSelector->y = new_y;
            break;
        default:
            printf("Could not find UI Type of %d.\n", element.type);
            break;
    }
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

UIElement CreateScrollSelectorElement(char key[], char label[], char *list_of_options[], int num_options, int x, int y) {
    ScrollSelector *scrollSelector = CreateScrollSelector(label, list_of_options, num_options, x, y);
    UIElement element = {UIT_SCROLLSELECTOR};
    strcpy(element.key, key);
    element.element.scrollSelector = scrollSelector;
    return element;
}


/***************
 * Layout Code *
 ***************/

int AutoLayout_Centered(UIElement **layout, int numElements, int width, int height, int padding) {
    const int t_heightOfSelector = 58;
    int t_heightOfTabElementsBlock = (padding * (numElements+1)) + (t_heightOfSelector * numElements);
    int t_firstElementOffset_y = (height - t_heightOfTabElementsBlock)/2;
    int t_otherElementHeights = 0;

    for (int i = 0; i < numElements; i++) {
        switch (layout[i]->type) {
            case UIT_SCROLLSELECTOR:
                layout[i]->element.scrollSelector->x = width/2;
                layout[i]->element.scrollSelector->y = t_firstElementOffset_y + t_otherElementHeights + padding;
                t_otherElementHeights += (t_heightOfSelector + padding);
                break;
            case UIT_BUTTON:
            case UIT_NUMLABEL:
            default:
                break;
        }
    }

    return t_heightOfTabElementsBlock;
}

int AutoLayout_SpaceBetween(UIElement **layout, int numElements, int width, int height) {
    // For now assume all elements are the same height, TODO: don't do this later
    const int t_heightOfSelector = 58;
    int t_padding = (height - (numElements*t_heightOfSelector))/(numElements+1);
    int t_otherElementHeights = 0;

    for (int i = 0; i < numElements; i++) {
        switch (layout[i]->type) {
            case UIT_SCROLLSELECTOR:
                layout[i]->element.scrollSelector->x = width/2;
                layout[i]->element.scrollSelector->y = t_otherElementHeights + t_padding;
                t_otherElementHeights += (t_heightOfSelector + t_padding);
                break;
            case UIT_BUTTON:
            case UIT_NUMLABEL:
            default:
                break;
        }
    }

    return -1;
}