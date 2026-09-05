int printf(const char *, ...);

// @rewrite-fn-begin
#if defined(__LP64__) || defined(_LP64)
int pointer_width_code(void) { return 64; }
#elif defined(__ILP32__) || defined(_ILP32)
int pointer_width_code(void) { return 32; }
#else
int pointer_width_code(void) { return 0; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", pointer_width_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(target_pointer_width = "64")]
// DIRECTIVES-DAG: #[cfg(target_pointer_width = "32")]
// DIRECTIVES-DAG: #[cfg(not(any(target_pointer_width = "64", target_pointer_width = "32")))]

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn pointer_width_code() -> i32 {
// COMMON-REWRITES-DAG:     64
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
