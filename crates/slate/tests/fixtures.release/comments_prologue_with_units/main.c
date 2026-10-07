/*
 * File prologue for the root unit.
 */
#include <stdio.h>

int helper(int x);

/* section: entry point */

int main(void) {
    printf("%d\n", helper(2));
    return 0;
}
