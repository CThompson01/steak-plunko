#include <stdlib.h>

int RandomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}