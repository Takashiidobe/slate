#include <stdio.h>

int main(void) {
  fprintf(stdout, "value: %d\n", 7);
  fputs("done\n", stdout);
  return 0;
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
// LOWERING-NEXT:     static mut stdout: *mut libc::FILE;
// LOWERING-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"value: %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fprintf({{_v[0-9]+}} as *mut libc::FILE, {{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"done\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fputs({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
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
// REWRITES-NEXT:     static mut stdout: *mut libc::FILE;
// REWRITES-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         fprintf(
// REWRITES-NEXT:             (unsafe { stdout }) as *mut libc::FILE,
// REWRITES-NEXT:             c"value: %d\n".as_ptr(),
// REWRITES-NEXT:             7 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { fputs(c"done\n".as_ptr(), (unsafe { stdout }) as *mut libc::FILE) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
