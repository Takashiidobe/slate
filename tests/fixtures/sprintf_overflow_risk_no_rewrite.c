#include <stdio.h>

int main(void) {
  char buf[4];
  // @rewrite-begin
  sprintf(buf, "%d", 7);
  puts(buf);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     sprintf(
// COMMON-REWRITES-DAG:         buf.as_mut_ptr() as *mut core::ffi::c_char,
// COMMON-REWRITES-DAG:         c"%d".as_ptr(),
// COMMON-REWRITES-DAG:         7 as i32,
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// COMMON-REWRITES-DAG: unsafe { puts(buf.as_mut_ptr() as *const core::ffi::c_char) };
// SLATE-FILECHECK-END common-rewrites
