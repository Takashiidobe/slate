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
// LOWERING-NEXT:     fn __assert_fail(
// LOWERING-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: i32,
// LOWERING-NEXT:         _3: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> !;
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
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"argc == 1\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"{{.*}}tests/fixtures/assert_runtime_true.c\0"
// LOWERING-NEXT:             .as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             __assert_fail(
// LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 {{__v[0-9]+}} as i32,
// LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT:     fn __assert_fail(
// REWRITES-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: i32,
// REWRITES-NEXT:         _3: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> !;
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
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"before\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i8 = c"argc == 1".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i8 = c"{{.*}}tests/fixtures/assert_runtime_true.c"
// REWRITES-NEXT:             .as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 6;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i8 = c"main".as_ptr() as *mut i8;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             __assert_fail(
// REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:                 {{__v[0-9]+}} as i32,
// REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"after\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
