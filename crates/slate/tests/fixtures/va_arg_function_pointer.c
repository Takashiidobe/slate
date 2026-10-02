#include <stdarg.h>
#include <stdio.h>

typedef int (*Callback)(int);
static int twice(int x) { return 2 * x; }
static int invoke(int x, ...) {
    va_list ap;
    va_start(ap, x);
    Callback first = va_arg(ap, Callback);
    Callback second = va_arg(ap, Callback);
    va_end(ap);
    return first(x) + (second ? second(x + 1) : 3);
}
int main(void) {
    printf("%d %d\n", invoke(4, twice, twice), invoke(7, twice, (Callback)0));
    return 0;
}
