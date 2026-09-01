#include <stdarg.h>
#include <stdio.h>

static void print_values(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}

int main(void) {
  print_values("%d %s\n", 42, "forwarded");
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
// LOWERING-NEXT:     fn vfprintf(
// LOWERING-NEXT:         _0: *mut libc::FILE,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: core::ffi::VaList<'_>,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" fn print_values({{arg[0-9]+}}: *mut i8, mut __slate_va_args: ...) {
// LOWERING-NEXT:     let mut args: core::ffi::VaList<'_>;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         args = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         vfprintf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             args.clone(),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"forwarded\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { print_values({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT:     fn vfprintf(
// REWRITES-NEXT:         _0: *mut libc::FILE,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: core::ffi::VaList<'_>,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" fn print_values({{arg[0-9]+}}: *mut i8, mut __slate_va_args: ...) {
// REWRITES-NEXT:     let mut args: core::ffi::VaList<'_>;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         args = __slate_va_args.clone();
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         vfprintf(
// REWRITES-NEXT:             (unsafe { stdout }) as *mut libc::FILE,
// REWRITES-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             args.clone(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         print_values(
// REWRITES-NEXT:             b"%d %s\n\0".as_ptr() as *mut i8,
// REWRITES-NEXT:             42 as i32,
// REWRITES-NEXT:             b"forwarded\0".as_ptr() as *mut i8,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
