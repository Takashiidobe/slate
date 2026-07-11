#include <stdio.h>

#if defined(_WIN32)
static int os_code(void) {
    return 10;
}
#elif defined(__linux__)
static int os_code(void) {
    return 20;
}
#elif defined(__APPLE__)
static int os_code(void) {
    return 30;
}
#else
static int os_code(void) {
    return 40;
}
#endif

int main(void) {
    printf("%d\n", os_code());
    return 0;
}
