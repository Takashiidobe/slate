#include <stdio.h>

int main(void) {
  // @rewrite-begin
  fprintf(stderr, "error: %d\n", 42);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"error: %d\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = 42;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     fprintf(
// REWRITES-DAG:         {{__v[0-9]+}} as *mut libc::FILE,
// REWRITES-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         {{__v[0-9]+}},
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
