#include <stdio.h>

__attribute__((destructor))
static void cleanup(void) {
    printf("destructor ran\n");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("early exit\n");
        return 7;
    }
    printf("main ran\n");
    return 0;
}
