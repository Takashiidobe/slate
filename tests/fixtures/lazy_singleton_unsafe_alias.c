#include <stdio.h>

static int cached_value = 0;
static int computed = 0;

static int compute(void) {
    return 42;
}

int get_value(void) {
    if (!computed) {
        cached_value = compute();
        computed = 1;
    }
    return cached_value;
}

/* Reads the payload outside the guarded init, so the singleton fixup must
   not fire: the static isn't exclusively owned by get_value anymore. */
int peek_value(void) {
    return cached_value;
}

int main(void) {
    printf("%d\n", get_value());
    printf("%d\n", peek_value());
    return 0;
}
