int printf(const char *, ...);

#if defined(__unix__) || defined(__unix)
static int unix_code(void) { return 1; }
#else
static int unix_code(void) { return 0; }
#endif

int main(void) {
  printf("%d\n", unix_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(unix)]
// DIRECTIVES-DAG: #[cfg(not(unix))]
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 1;
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 0;
