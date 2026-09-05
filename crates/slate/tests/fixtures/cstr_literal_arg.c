#include <stdio.h>

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  puts("hello");
  // @lowering-end
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i8 = b"hello\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"hello".as_ptr() as *mut i8;
// REWRITES-DAG: unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END rewrites
