#include <stdio.h>

int main(void) {
  // @rewrite-begin
  fprintf(stderr, "error: %d\n", 42);
  // @rewrite-end
  return 0;
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"error: %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 42;
// REWRITES-DAG: unsafe { fprintf({{_v[0-9]+}} as *mut libc::FILE, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
