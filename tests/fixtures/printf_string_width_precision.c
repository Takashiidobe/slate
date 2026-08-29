#include <stdio.h>

int main(void) {
  // @rewrite-begin
  printf("%5s|%-5s|%.1s|%6.1s\n", "hi", "hi", "hi", "hi");
  // @rewrite-end
  return 0;
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%5s|%-5s|%.1s|%6.1s\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
