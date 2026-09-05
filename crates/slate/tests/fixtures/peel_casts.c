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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { puts({{__v[0-9]+}} as *const core::ffi::c_char) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"hello\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"hello\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe { puts(c"hello".as_ptr()) };
// SLATE-FILECHECK-END common-rewrites
