#include <stdio.h>

static int bump_through_pointer(int value) {
    int local = value;
    int *ptr = &local;
    *ptr = *ptr + 3;
    return local;
}

static int add_into_pointer(int *slot, int amount) {
    *slot = *slot + amount;
    return *slot;
}

static int pick_with_pointer_arithmetic(int index) {
    int values[4];
    int *ptr = values;
    values[0] = 4;
    values[1] = 8;
    values[2] = 12;
    values[3] = 16;
    return *(ptr + index);
}

int main(void) {
    int total = 10;
    printf("%d\n", bump_through_pointer(5));
    printf("%d\n", add_into_pointer(&total, 7));
    printf("%d\n", total);
    printf("%d\n", pick_with_pointer_arithmetic(2));
    return 0;
}
