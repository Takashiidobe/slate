#include <stdio.h>

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  fputs("bye\n", stderr);
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"bye\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { fputs({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *mut libc::FILE) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"bye\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { fputs({{_v[0-9]+}} as *const i8, (unsafe { stderr }) as *mut libc::FILE) };
// SLATE-FILECHECK-END rewrites
