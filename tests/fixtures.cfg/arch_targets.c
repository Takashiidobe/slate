#include <stdio.h>

#if defined(__x86_64__) || defined(_M_X64)
static int arch_code(void) {
    return 64;
}
#elif defined(__aarch64__) || defined(_M_ARM64)
static int arch_code(void) {
    return 128;
}
#elif defined(__arm__) || defined(_M_ARM)
static int arch_code(void) {
    return 32;
}
#else
static int arch_code(void) {
    return 0;
}
#endif

int main(void) {
    printf("%d\n", arch_code());
    return 0;
}
