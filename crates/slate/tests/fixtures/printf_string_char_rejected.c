#include <stdio.h>

int main(void) {
  char buf[4] = "hey";
  // @rewrite-begin
  printf("%s\n", buf);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"%s\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
