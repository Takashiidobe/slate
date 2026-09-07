#include <stdio.h>

int main(void) {
  // @rewrite-begin
  // @lowering-begin
  int a = 2;
  int b = 3;
  printf("sum: %d and %d\n", a, b);
  printf("hex: %x\n", a);
  printf("no newline: %d", a + b);
  // @lowering-end
  // @rewrite-end
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"sum: %d and %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"sum: %d and %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"hex: %x\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"hex: %x\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"no newline: %d\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"no newline: %d\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG: println!("sum: {} and {}", {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG: let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-DAG: let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-DAG: unsafe { printf(c"hex: %x\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-DAG: unsafe { fflush(std::ptr::null_mut()) };
// REWRITES-DAG: print!("no newline: {}", {{__v[0-9]+}} + {{__v[0-9]+}});
// REWRITES-DAG: let _ = std::io::Write::flush(&mut std::io::stdout());
// SLATE-FILECHECK-END rewrites
