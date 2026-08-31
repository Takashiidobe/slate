#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
  int r = (argc == 1) ? 0 : (abort(), 0);
  printf("%d\n", r);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args().map(|arg| std::ffi::CString::new(arg).unwrap()).collect();
// LOWERING-NEXT:     let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage.iter().map(|arg| arg.as_ptr() as *mut i8).collect();
// LOWERING-NEXT:     __slate_argv_ptrs.push(std::ptr::null_mut());
// LOWERING-NEXT:     let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// LOWERING-NEXT:     let {{arg[0-9]+}}: *mut *mut i8 = __slate_argv_ptrs.as_mut_ptr();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         unsafe { abort() };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args().map(|arg| std::ffi::CString::new(arg).unwrap()).collect();
// REWRITES-NEXT: let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage.iter().map(|arg| arg.as_ptr() as *mut i8).collect();
// REWRITES-NEXT: __slate_argv_ptrs.push(std::ptr::null_mut());
// REWRITES-NEXT: let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// REWRITES-NEXT: __slate_argv_ptrs.as_mut_ptr();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         unsafe { std::process::abort() };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
