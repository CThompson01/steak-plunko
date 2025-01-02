#ifndef GLOBAL_H
#define GLOBAL_H
#include "raylib.h"

enum Screen {
    GAME,
    OPTIONS,
    CLOSE_GAME
};

int randomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

#endif