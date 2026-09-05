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

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn arm_endian_code() -> i32 {
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     0
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     200
// SLATE-FILECHECK-END rewrites-aarch64-gnu
