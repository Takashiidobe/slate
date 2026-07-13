int printf(const char *, ...);

#if defined(__LP64__) || defined(_LP64)
static int pointer_width_code(void) {
    return 64;
}
#elif defined(__ILP32__) || defined(_ILP32)
static int pointer_width_code(void) {
    return 32;
}
#else
static int pointer_width_code(void) {
    return 0;
}
#endif

int main(void) {
    printf("%d\n", pointer_width_code());
    return 0;
}
