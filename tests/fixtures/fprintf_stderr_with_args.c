#include <stdio.h>

int main(void) {
  // @rewrite-begin
  fprintf(stderr, "error: %d\n", 42);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     fprintf(
// COMMON-REWRITES-DAG:         (unsafe { stderr }) as *mut libc::FILE,
// COMMON-REWRITES-DAG:         c"error: %d\n".as_ptr(),
// COMMON-REWRITES-DAG:         42 as i32,
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites
