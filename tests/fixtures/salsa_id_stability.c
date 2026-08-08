#include <stdio.h>

static int stable_function(int value) {
    return value * 3 + 1;
}

int main(void) {
    printf("%d\n", stable_function(7));
    return 0;
}
