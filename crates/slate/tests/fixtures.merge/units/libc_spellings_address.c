#include <ctype.h>
#include <errno.h>

static int count(int (*test)(int)) {
    int total = 0;
    for (int c = -1; c < 256; c++) {
        if (test(c)) {
            total++;
        }
    }
    return total;
}

int count_digits(void) {
    errno = 0;
    return count(isdigit) + errno + (isgraph('a') ? 1 : 0);
}
