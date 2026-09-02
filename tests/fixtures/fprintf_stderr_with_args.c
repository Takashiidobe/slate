#include <stdio.h>

int main(void) {
  // @rewrite-begin
  fprintf(stderr, "error: %d\n", 42);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     fprintf(
// REWRITES-DAG:         (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-DAG:         c"error: %d\n".as_ptr(),
// REWRITES-DAG:         42 as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
