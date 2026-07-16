#include <stdio.h>

__attribute__((constructor))
static void greet(void) {
    printf("constructor ran\n");
}

int main(void) {
    printf("main ran\n");
    return 0;
}
