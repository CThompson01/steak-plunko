#include <stdlib.h>

int randomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}