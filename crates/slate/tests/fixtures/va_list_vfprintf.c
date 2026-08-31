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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut stdout: *mut libc::FILE;
// LOWERING-NEXT:     fn vfprintf(_0: *mut libc::FILE, _1: *const i8, _2: core::ffi::VaList<'_>) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" fn print_values({{arg[0-9]+}}: *mut i8, mut __slate_va_args: ...) {
// LOWERING-NEXT:     let mut args: core::ffi::VaList<'_>;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         args = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { vfprintf({{_v[0-9]+}} as *mut libc::FILE, {{arg[0-9]+}} as *const i8, args.clone()) };
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
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut stdout: *mut libc::FILE;
// REWRITES-NEXT:     fn vfprintf(_0: *mut libc::FILE, _1: *const i8, _2: core::ffi::VaList<'_>) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" fn print_values({{arg[0-9]+}}: *mut i8, mut __slate_va_args: ...) {
// REWRITES-NEXT: let mut args: core::ffi::VaList<'_>;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         args = __slate_va_args.clone();
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe { vfprintf((unsafe { stdout }) as *mut libc::FILE, {{arg[0-9]+}} as *const i8, args.clone()) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"forwarded\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { print_values({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
