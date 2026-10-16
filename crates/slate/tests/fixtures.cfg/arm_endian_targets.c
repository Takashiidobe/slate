int printf(const char *, ...);

// @check-fn-begin
#if defined(__ARMEB__) || defined(__AARCH64EB__)
int arm_endian_code(void) { return 100; }
#elif defined(__ARMEL__) || defined(__AARCH64EL__)
int arm_endian_code(void) { return 200; }
#else
int arm_endian_code(void) { return 0; }
#endif
// @check-fn-end

int main(void) {
  printf("%d\n", arm_endian_code());
  return 0;
}
