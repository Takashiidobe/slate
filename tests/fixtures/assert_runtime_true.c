#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("before\n");
  assert(argc == 1);
  printf("after\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn __assert_fail(_0: *const i8, _1: *const i8, _2: i32, _3: *const i8) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args().map(|arg| std::ffi::CString::new(arg).unwrap()).collect();
// LOWERING-NEXT:     let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage.iter().map(|arg| arg.as_ptr() as *mut i8).collect();
// LOWERING-NEXT:     __slate_argv_ptrs.push(std::ptr::null_mut());
// LOWERING-NEXT:     let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// LOWERING-NEXT:     let {{arg[0-9]+}}: *mut *mut i8 = __slate_argv_ptrs.as_mut_ptr();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"argc == 1\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"{{.*}}tests/fixtures/assert_runtime_true.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         unsafe { __assert_fail({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn __assert_fail(_0: *const i8, _1: *const i8, _2: i32, _3: *const i8) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args().map(|arg| std::ffi::CString::new(arg).unwrap()).collect();
// REWRITES-NEXT: let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage.iter().map(|arg| arg.as_ptr() as *mut i8).collect();
// REWRITES-NEXT: __slate_argv_ptrs.push(std::ptr::null_mut());
// REWRITES-NEXT: let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// REWRITES-NEXT: let {{arg[0-9]+}}: *mut *mut i8 = __slate_argv_ptrs.as_mut_ptr();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = true;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"argc == 1\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"{{.*}}tests/fixtures/assert_runtime_true.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         unsafe { __assert_fail({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
