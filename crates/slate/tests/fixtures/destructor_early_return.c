#include <stdio.h>

__attribute__((destructor)) static void cleanup(void) {
  printf("destructor ran\n");
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("early exit\n");
    return 7;
  }
  printf("main ran\n");
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
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn cleanup() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"destructor ran\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args()
// LOWERING-NEXT:         .map(|arg| std::ffi::CString::new(arg).unwrap())
// LOWERING-NEXT:         .collect();
// LOWERING-NEXT:     let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage
// LOWERING-NEXT:         .iter()
// LOWERING-NEXT:         .map(|arg| arg.as_ptr() as *mut i8)
// LOWERING-NEXT:         .collect();
// LOWERING-NEXT:     __slate_argv_ptrs.push(std::ptr::null_mut());
// LOWERING-NEXT:     let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// LOWERING-NEXT:     let {{arg[0-9]+}}: *mut *mut i8 = __slate_argv_ptrs.as_mut_ptr();
// LOWERING-NEXT:     let mut argc: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     argc = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = argc;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"early exit\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             cleanup();
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"main ran\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     cleanup();
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
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn cleanup() {
// REWRITES-NEXT:     unsafe { printf(c"destructor ran\n".as_ptr()) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args()
// REWRITES-NEXT:         .map(|arg| std::ffi::CString::new(arg).unwrap())
// REWRITES-NEXT:         .collect();
// REWRITES-NEXT:     let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage
// REWRITES-NEXT:         .iter()
// REWRITES-NEXT:         .map(|arg| arg.as_ptr() as *mut i8)
// REWRITES-NEXT:         .collect();
// REWRITES-NEXT:     __slate_argv_ptrs.push(std::ptr::null_mut());
// REWRITES-NEXT:     let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// REWRITES-NEXT:     __slate_argv_ptrs.as_mut_ptr();
// REWRITES-NEXT:     let mut argc: i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = argc == 1;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { printf(c"early exit\n".as_ptr()) };
// REWRITES-NEXT:         __retval = 7;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = __retval;
// REWRITES-NEXT:         cleanup();
// REWRITES-NEXT:         std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"main ran\n".as_ptr()) };
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// REWRITES-NEXT:     cleanup();
// REWRITES-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
