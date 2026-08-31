#include <stdio.h>

#if defined(NDEBUG)
static int debug_code(void) { return 0; }
#else
static int debug_code(void) { return 1; }
#endif

int main(void) {
  printf("%d\n", debug_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(not(debug_assertions))]
// DIRECTIVES-DAG: #[cfg(debug_assertions)]
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}0;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}1;
