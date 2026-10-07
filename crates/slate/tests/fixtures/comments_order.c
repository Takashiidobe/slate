#include <stdio.h>

/* ---- first section ---- */

int a1 = 1;

static int s1(void) { return a1; }

int e1(void) { return s1(); }

/* ---- second section ---- */

int a2 = 2;

/* second static */
static int s2(void) { return a2; }

int e2(void) { return s2(); }

int main(void) {
    printf("%d\n", e1() + e2());
    return 0;
}
