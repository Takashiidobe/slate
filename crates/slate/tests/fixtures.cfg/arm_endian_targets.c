int printf(const char *, ...);

#if defined(__ARMEB__) || defined(__AARCH64EB__)
static int arm_endian_code(void) {
    return 100;
}
#elif defined(__ARMEL__) || defined(__AARCH64EL__)
static int arm_endian_code(void) {
    return 200;
}
#else
static int arm_endian_code(void) {
    return 0;
}
#endif

int main(void) {
    printf("%d\n", arm_endian_code());
    return 0;
}
