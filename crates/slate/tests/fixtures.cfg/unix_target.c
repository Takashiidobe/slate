int printf(const char *, ...);

// @rewrite-fn-begin
#if defined(__unix__) || defined(__unix)
int unix_code(void) { return 1; }
#else
int unix_code(void) { return 0; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", unix_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(unix)]
// DIRECTIVES-DAG: #[cfg(not(unix))]

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn unix_code() -> i32 {
// COMMON-REWRITES-DAG:     1
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
