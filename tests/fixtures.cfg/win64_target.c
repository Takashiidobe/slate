int printf(const char *, ...);

#if defined(_WIN64)
static int win64_code(void) { return 64; }
#else
static int win64_code(void) { return 0; }
#endif

int main(void) {
  printf("%d\n", win64_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(all(windows, target_pointer_width = "64"))]
// DIRECTIVES-DAG: #[cfg(not(all(windows, target_pointer_width = "64")))]
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}64;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}0;
