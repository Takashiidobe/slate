int printf(const char *, ...);

#ifdef MY_FEATURE
static int feature_code(void) {
    return 10;
}
#else
static int feature_code(void) {
    return 20;
}
#endif

int main(void) {
    printf("%d\n", feature_code());
    return 0;
}
