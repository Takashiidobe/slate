#include <stdio.h>

// file-scope `static`: internal linkage, private to this unit. `other.c`
// defines its own distinct `base`/`local` under the same names.
static int base = 10;
static int local(int x) { return x + base; }

// external-linkage function defined in the sibling unit.
int compute(int x);

int main(void) {
    printf("%d %d\n", local(5), compute(3));
    return 0;
}
