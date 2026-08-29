#include <stdio.h>

int main(void) {
  int value;
  // @rewrite-not
  value = 40 + 2;
  // @rewrite-not
  // @rewrite
  printf("%d\n", value);
  // @rewrite
  // @rewrite
  puts("_v9 anon_4 anon_struct_i32");
  // @rewrite
  return 0;
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-NOT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NOT: value = {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, value) };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"_v9 anon_4 anon_struct_i32\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const i8) };
// SLATE-FILECHECK-END rewrites
