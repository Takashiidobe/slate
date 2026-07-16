#include <stdio.h>

__attribute__((destructor))
static void farewell(void) {
    printf("destructor ran\n");
}

int main(void) {
    printf("main ran\n");
    return 0;
}
