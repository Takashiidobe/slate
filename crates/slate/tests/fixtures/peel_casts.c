#include <stdio.h>

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  puts("hello");
  // @lowering-end
  // @rewrite-end
  printf("%d\n", 42);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"hello\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"hello\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { puts(c"hello".as_ptr()) };
// SLATE-FILECHECK-END rewrites
