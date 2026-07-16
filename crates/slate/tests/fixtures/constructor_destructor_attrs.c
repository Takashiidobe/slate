#include <stdio.h>

__attribute__((constructor))
static void register_default(void) {
    printf("ctor: default\n");
}

__attribute__((destructor))
static void cleanup_default(void) {
    printf("dtor: default\n");
}

int main(void) {
    printf("main\n");
    return 0;
}
