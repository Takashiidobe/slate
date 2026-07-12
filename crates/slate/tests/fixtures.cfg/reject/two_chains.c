#include <stdio.h>

#if defined(__linux__)
static int a(void) {
    return 1;
}
#else
static int a(void) {
    return 2;
}
#endif

#if defined(NDEBUG)
static int b(void) {
    return 3;
}
#else
static int b(void) {
    return 4;
}
#endif

int main(void) {
    printf("%d\n", a() + b());
    return 0;
}
