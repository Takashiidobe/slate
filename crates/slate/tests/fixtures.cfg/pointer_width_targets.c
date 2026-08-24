int printf(const char *, ...);

#if defined(__LP64__) || defined(_LP64)
static int pointer_width_code(void) { return 64; }
#elif defined(__ILP32__) || defined(_ILP32)
static int pointer_width_code(void) { return 32; }
#else
static int pointer_width_code(void) { return 0; }
#endif

int main(void) {
  printf("%d\n", pointer_width_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(target_pointer_width = "64")]
// DIRECTIVES-DAG: #[cfg(target_pointer_width = "32")]
// DIRECTIVES-DAG: #[cfg(not(any(target_pointer_width = "64", target_pointer_width = "32")))]
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 64;
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 32;
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 0;
