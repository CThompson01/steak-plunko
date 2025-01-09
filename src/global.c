#include <stdlib.h>

int RandomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

unsigned int SumTo(unsigned int n) {
    return (n * (n+1))/2;
}