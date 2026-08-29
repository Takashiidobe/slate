#include <stdio.h>

static char *names[] = {"alpha", "beta"};

int main(void) {
  // @rewrite-begin
  printf("%s %s\n", names[0], names[1]);
  // @rewrite-end
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%s %s\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { (*names)[({{_v[0-9]+}} as usize)] }, unsafe { (*names)[({{_v[0-9]+}} as usize)] }) };
// SLATE-FILECHECK-END rewrites
