int printf(const char *, ...);

#if defined(_WIN64)
static int win64_code(void) {
    return 64;
}
#else
static int win64_code(void) {
    return 0;
}
#endif

int main(void) {
    printf("%d\n", win64_code());
    return 0;
}
