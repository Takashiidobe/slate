#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("before\n");
  assert(argc == 1);
  printf("after\n");
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
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn __assert_fail(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: i32,
// COMMON-LOWERING-NEXT:         _3: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> !;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args()
// COMMON-LOWERING-NEXT:         .map(|arg| std::ffi::CString::new(arg).unwrap())
// COMMON-LOWERING-NEXT:         .collect();
// COMMON-LOWERING-NEXT:         .iter()
// COMMON-LOWERING-NEXT:         .collect();
// COMMON-LOWERING-NEXT:     __slate_argv_ptrs.push(std::ptr::null_mut());
// COMMON-LOWERING-NEXT:     let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             __assert_fail(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage
// LOWERING-X86_64-GNU-NEXT:         .map(|arg| arg.as_ptr() as *mut i8)
// LOWERING-X86_64-GNU-NEXT:     let {{arg[0-9]+}}: *mut *mut i8 = __slate_argv_ptrs.as_mut_ptr();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"argc == 1\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"{{.*}}tests/fixtures/assert_runtime_true.c\0"
// LOWERING-X86_64-GNU-NEXT:             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut __slate_argv_ptrs: Vec<*mut u8> = __slate_argv_storage
// LOWERING-AARCH64-GNU-NEXT:         .map(|arg| arg.as_ptr() as *mut u8)
// LOWERING-AARCH64-GNU-NEXT:     let {{arg[0-9]+}}: *mut *mut u8 = __slate_argv_ptrs.as_mut_ptr();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"before\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"argc == 1\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"{{.*}}tests/fixtures/assert_runtime_true.c\0"
// LOWERING-AARCH64-GNU-NEXT:             .as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"main\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"after\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn __assert_fail(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: i32,
// COMMON-REWRITES-NEXT:         _3: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> !;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args()
// COMMON-REWRITES-NEXT:         .map(|arg| std::ffi::CString::new(arg).unwrap())
// COMMON-REWRITES-NEXT:         .collect();
// COMMON-REWRITES-NEXT:         .iter()
// COMMON-REWRITES-NEXT:         .collect();
// COMMON-REWRITES-NEXT:     __slate_argv_ptrs.push(std::ptr::null_mut());
// COMMON-REWRITES-NEXT:     let {{arg[0-9]+}}: i32 = __slate_argv_storage.len() as i32;
// COMMON-REWRITES-NEXT:     __slate_argv_ptrs.as_mut_ptr();
// COMMON-REWRITES-NEXT:     unsafe { printf(c"before\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             __assert_fail(
// COMMON-REWRITES-NEXT:                 c"argc == 1".as_ptr(),
// COMMON-REWRITES-NEXT:                 c"{{.*}}tests/fixtures/assert_runtime_true.c".as_ptr(),
// COMMON-REWRITES-NEXT:                 6 as i32,
// COMMON-REWRITES-NEXT:                 c"main".as_ptr(),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"after\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage
// REWRITES-X86_64-GNU-NEXT:         .map(|arg| arg.as_ptr() as *mut i8)
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut __slate_argv_ptrs: Vec<*mut u8> = __slate_argv_storage
// REWRITES-AARCH64-GNU-NEXT:         .map(|arg| arg.as_ptr() as *mut u8)
// SLATE-FILECHECK-END rewrites-aarch64-gnu
