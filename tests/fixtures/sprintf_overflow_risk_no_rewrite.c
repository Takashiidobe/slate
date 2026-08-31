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
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-DAG: unsafe { sprintf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-DAG: unsafe { puts({{_v[0-9]+}} as *const i8) };
// SLATE-FILECHECK-END rewrites
