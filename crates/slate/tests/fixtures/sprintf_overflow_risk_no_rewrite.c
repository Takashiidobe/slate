#include <stdio.h>

int main(void) {
  char buf[4];
  // @rewrite-begin
  sprintf(buf, "%d", 7);
  puts(buf);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     sprintf(
// REWRITES-DAG:         buf.as_mut_ptr() as *mut core::ffi::c_char,
// REWRITES-DAG:         c"%d".as_ptr(),
// REWRITES-DAG:         7 as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: unsafe { puts(buf.as_mut_ptr() as *const core::ffi::c_char) };
// SLATE-FILECHECK-END rewrites
