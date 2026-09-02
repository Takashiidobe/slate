#include <stdio.h>

// @rewrite-fn-begin
#if defined(NDEBUG)
int debug_code(void) { return 0; }
#else
int debug_code(void) { return 1; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", debug_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(not(debug_assertions))]
// DIRECTIVES-DAG: #[cfg(debug_assertions)]

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn debug_code() -> i32 {
// REWRITES-DAG:     1
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
