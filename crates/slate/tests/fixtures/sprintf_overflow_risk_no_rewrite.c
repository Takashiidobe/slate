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
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"%d".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = 7;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     sprintf(
// REWRITES-DAG:         {{__v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{__v[0-9]+}},
// REWRITES-DAG:     )
// REWRITES-DAG: };
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-DAG: unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END rewrites
