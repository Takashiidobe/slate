#include <stdio.h>

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  fputs("bye\n", stderr);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"bye\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"bye\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { fputs({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as *mut libc::FILE) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { fputs(c"bye\n".as_ptr(), (unsafe { stderr }) as *mut libc::FILE) };
// SLATE-FILECHECK-END rewrites
