int printf(const char *, ...);

// @check-fn-begin
#if defined(__unix__) || defined(__unix)
int unix_code(void) { return 1; }
#else
int unix_code(void) { return 0; }
#endif
// @check-fn-end

int main(void) {
  printf("%d\n", unix_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(unix)]
// DIRECTIVES-DAG: #[cfg(not(unix))]
