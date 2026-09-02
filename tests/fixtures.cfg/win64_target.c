int printf(const char *, ...);

// @rewrite-fn-begin
#if defined(_WIN64)
int win64_code(void) { return 64; }
#else
int win64_code(void) { return 0; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", win64_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(all(windows, target_pointer_width = "64"))]
// DIRECTIVES-DAG: #[cfg(not(all(windows, target_pointer_width = "64")))]

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn win64_code() -> i32 {
// REWRITES-DAG:     0
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
