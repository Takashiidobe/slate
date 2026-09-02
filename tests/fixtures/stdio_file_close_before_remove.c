#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_stdio_close_before_remove.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);
  remove("slate_stdio_close_before_remove.tmp");
  puts("done");
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// LOWERING-NEXT:     fn puts(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate_stdio_close_before_remove.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe {
// LOWERING-NEXT:         fopen(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     f = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = f;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"owned\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fputs({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fclose({{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate_stdio_close_before_remove.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { remove({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"done\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { puts({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const core::ffi::c_char, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut f: *mut libc::FILE = unsafe {
// REWRITES-NEXT:         fopen(
// REWRITES-NEXT:             c"slate_stdio_close_before_remove.tmp".as_ptr(),
// REWRITES-NEXT:             c"w".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = f != std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { puts(c"open-fail".as_ptr()) };
// REWRITES-NEXT:         __retval = 0;
// REWRITES-NEXT:         std::process::exit(__retval as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { fputs(c"owned\n".as_ptr(), f as *mut libc::FILE) };
// REWRITES-NEXT:     unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT:     unsafe { remove(c"slate_stdio_close_before_remove.tmp".as_ptr()) };
// REWRITES-NEXT:     unsafe { puts(c"done".as_ptr()) };
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
