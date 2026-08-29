#include <stdio.h>

int main(void) {
  char buf[4] = "hey";
  // @rewrite-begin
  printf("%s\n", buf);
  // @rewrite-end
  return 0;
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
