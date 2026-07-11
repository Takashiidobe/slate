#include <stdio.h>

struct Triple {
    int x;
    int y;
    int z;
};

int main(void) {
    struct Triple full = {1, 2, 3};
    struct Triple partial = {4, 5};
    struct Triple designated = {.z = 9, .x = 7};
    printf("%d %d %d\n", full.x, full.y, full.z);
    printf("%d %d %d\n", partial.x, partial.y, partial.z);
    printf("%d %d %d\n", designated.x, designated.y, designated.z);

    struct Triple copy = full;
    copy.x = 42;
    printf("%d %d\n", full.x, copy.x);
    return 0;
}
