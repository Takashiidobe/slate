int printf(const char *, ...);

#if defined(_WIN32)
static int os_code(void) {
    return 10;
}
#elif defined(__ANDROID__)
static int os_code(void) {
    return 25;
}
#elif defined(__linux__)
static int os_code(void) {
    return 20;
}
#elif defined(__APPLE__)
static int os_code(void) {
    return 30;
}
#elif defined(__FreeBSD__)
static int os_code(void) {
    return 35;
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
