#include <stdlib.h>

int RandomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

unsigned int SumTo(unsigned int n) {
    int sum = 0;
    while (n > 0) {
        sum += n;
        n--;
    }
    return sum;
}