int printf(const char *, ...);

// @rewrite-fn-begin
#if defined(__ARMEB__) || defined(__AARCH64EB__)
int arm_endian_code(void) { return 100; }
#elif defined(__ARMEL__) || defined(__AARCH64EL__)
int arm_endian_code(void) { return 200; }
#else
int arm_endian_code(void) { return 0; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", arm_endian_code());
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn arm_endian_code() -> i32 {
// REWRITES-DAG:     0
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
