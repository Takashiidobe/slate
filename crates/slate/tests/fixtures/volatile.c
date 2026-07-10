#include <stdio.h>

static int volatile_local(int value) {
    volatile int slot = value;
    slot = slot + 3;
    return slot;
}

int main(void) {
    printf("%d\n", volatile_local(4));
    return 0;
}
