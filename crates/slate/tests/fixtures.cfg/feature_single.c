int printf(const char *, ...);

// @rewrite-fn-begin
#ifdef MY_FEATURE
int feature_code(void) { return 10; }
#else
int feature_code(void) { return 20; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", feature_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(feature = "my_feature")]
// DIRECTIVES-DAG: #[cfg(not(feature = "my_feature"))]

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn feature_code() -> i32 {
// COMMON-REWRITES-DAG:     20
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
