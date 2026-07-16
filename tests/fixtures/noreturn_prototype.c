#include <stdio.h>
#include <stdlib.h>

void bail(int code) __attribute__((noreturn));

void bail(int code) {
    printf("bailing with %d\n", code);
    exit(code);
}

int main(void) {
    printf("main\n");
    bail(7);
}
