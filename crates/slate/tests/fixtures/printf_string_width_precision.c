#include <stdio.h>

int main(void) {
  // @rewrite-begin
  printf("%5s|%-5s|%.1s|%6.1s\n", "hi", "hi", "hi", "hi");
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"%5s|%-5s|%.1s|%6.1s\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"hi".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"hi".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"hi".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"hi".as_ptr() as *mut i8;
// REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
