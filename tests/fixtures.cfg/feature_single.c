int printf(const char *, ...);

#ifdef MY_FEATURE
static int feature_code(void) { return 10; }
#else
static int feature_code(void) { return 20; }
#endif

int main(void) {
  printf("%d\n", feature_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(feature = "my_feature")]
// DIRECTIVES-DAG: #[cfg(not(feature = "my_feature"))]
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 10;
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 20;
