#include <stdio.h>

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  printf("%d %d\n", 7, 8);
  // @lowering-end
  // @rewrite-end
  printf("%ld\n", 42L);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 7;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { printf(c"%d %d\n".as_ptr(), 7 as i32, 8 as i32) };
// SLATE-FILECHECK-END rewrites
