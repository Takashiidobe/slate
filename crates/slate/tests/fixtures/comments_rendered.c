/*
 * Prologue block.
 * ==========
 */
#include <stdio.h>

/** Doc for point. */
struct point {
    int x; /* the x */
    /* the y */
    int y;
};

/* limit of things */
static const int limit = 3; /* trailing limit */

enum color {
    RED,   /* red one */
    GREEN, /* green one */
};

/**
 * Adds.
 *
 * @param a left
 */
static int add(int a /* left */, int b /* right */) {
    int r = a + b; /* sum */
    /* FALLTHROUGH */
    return r;
}

int main(void) {
    struct point p = {1, 2};
    printf("%d %d\n", add(p.x, p.y), limit + GREEN);
    return 0;
}
