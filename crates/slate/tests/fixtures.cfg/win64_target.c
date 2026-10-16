int printf(const char *, ...);

// @check-fn-begin
#if defined(_WIN64)
int win64_code(void) { return 64; }
#else
int win64_code(void) { return 0; }
#endif
// @check-fn-end

int main(void) {
  printf("%d\n", win64_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(all(windows, target_pointer_width = "64"))]
// DIRECTIVES-DAG: #[cfg(not(all(windows, target_pointer_width = "64")))]
