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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     static mut stdout: *mut libc::FILE;
// COMMON-LOWERING-NEXT:     fn vfprintf(
// COMMON-LOWERING-NEXT:         _0: *mut libc::FILE,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: core::ffi::VaList<'_>,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     unsafe { print_values({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut args: core::ffi::VaList<'_>;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         args = __slate_va_args.clone();
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe { stdout };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         vfprintf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             args.clone(),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"forwarded\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C-unwind" fn print_values({{arg[0-9]+}}: *mut i8, mut __slate_va_args: ...) {
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"forwarded\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-EMPTY:
// LOWERING-AARCH64-GNU-NEXT: unsafe extern "C-unwind" fn print_values({{arg[0-9]+}}: *mut u8, mut __slate_va_args: ...) {
// LOWERING-AARCH64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     static mut stdout: *mut libc::FILE;
// COMMON-REWRITES-NEXT:     fn vfprintf(
// COMMON-REWRITES-NEXT:         _0: *mut libc::FILE,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: core::ffi::VaList<'_>,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         print_values(
// COMMON-REWRITES-NEXT:             42 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let mut args: core::ffi::VaList<'_>;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         args = __slate_va_args.clone();
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         vfprintf(
// COMMON-REWRITES-NEXT:             (unsafe { stdout }) as *mut libc::FILE,
// COMMON-REWRITES-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             args.clone(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:             c"%d %s\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             c"forwarded".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C-unwind" fn print_values({{arg[0-9]+}}: *mut i8, mut __slate_va_args: ...) {
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT:             c"%d %s\n".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             c"forwarded".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-EMPTY:
// REWRITES-AARCH64-GNU-NEXT: unsafe extern "C-unwind" fn print_values({{arg[0-9]+}}: *mut u8, mut __slate_va_args: ...) {
// REWRITES-AARCH64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
