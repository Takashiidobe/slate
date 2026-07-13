int printf(const char *, ...);

#ifdef FIRST_FEATURE
static int first_code(void) {
    return 1;
}
#else
static int first_code(void) {
    return 2;
}
#endif

#ifdef SECOND_FEATURE
static int second_code(void) {
    return 3;
}
#else
static int second_code(void) {
    return 4;
}
#endif

int main(void) {
    printf("%d %d\n", first_code(), second_code());
    return 0;
}
