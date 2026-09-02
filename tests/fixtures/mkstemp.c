#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char path[] = "slate-XXXXXX";
  int  fd     = mkstemp(path);
  printf("%d\n", fd >= 0);
  if (fd >= 0) {
    close(fd);
    unlink(path);
  }
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn mkstemp(_0: *mut core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn close(_0: i32) -> i32;
// LOWERING-NEXT:     fn unlink(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut path: [i8; 13] = [0; 13];
// LOWERING-NEXT:     let mut fd: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     path = [115, 108, 97, 116, 101, 45, 88, 88, 88, 88, 88, 88, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = path.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { mkstemp({{_v[0-9]+}} as *mut core::ffi::c_char) };
// LOWERING-NEXT:     fd = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = fd;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = fd;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = fd;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { close({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = path.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { unlink({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn mkstemp(_0: *mut core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn close(_0: i32) -> i32;
// REWRITES-NEXT:     fn unlink(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut path: [i8; 13] = [115, 108, 97, 116, 101, 45, 88, 88, 88, 88, 88, 88, 0];
// REWRITES-NEXT:     let mut fd: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = path.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     fd = unsafe { mkstemp({{_v[0-9]+}} as *mut core::ffi::c_char) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = fd >= 0;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = fd >= 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { close(fd as i32) };
// REWRITES-NEXT:         unsafe { unlink(path.as_mut_ptr() as *const core::ffi::c_char) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
