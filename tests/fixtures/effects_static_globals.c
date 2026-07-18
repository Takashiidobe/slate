#include <stdio.h>

static int counter = 2;

static int bump_counter(void) {
    counter = counter + 1;
    return counter;
}

int main(void) {
    printf("%d\n", bump_counter());
    printf("%d\n", bump_counter());
    return 0;
}
