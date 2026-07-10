#include <stdio.h>

struct Pair {
    int left;
    int right;
};

union Slot {
    int value;
    unsigned int raw;
};

static int sizeof_values(void) {
    int values[3];
    return sizeof(int) + sizeof(values) + sizeof(struct Pair) + sizeof(union Slot);
}

static int sizeof_expr(void) {
    int value = 0;
    return sizeof value;
}

int main(void) {
    printf("%d\n", sizeof_values());
    printf("%d\n", sizeof_expr());
    return 0;
}
