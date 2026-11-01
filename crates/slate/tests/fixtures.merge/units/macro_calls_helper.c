#include <stdio.h>

static int same_name(int value) {
    return value + 100;
}

#define CALL_SAME(a) { printf("%d\n", same_name(a)); }

int exported_bump(int *value) {
    *value += 10;
    return *value;
}

int exported_length(const char *text) {
    int count = 0;
    while (text[count]) {
        count++;
    }
    return count;
}

int helper_run(void) {
    int local = 5;
    CALL_SAME(local);
    return local;
}
