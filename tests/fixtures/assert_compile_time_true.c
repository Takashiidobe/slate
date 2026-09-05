#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("before\n");
  assert(5 == 5);
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
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"before\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"5 == 5\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 =
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"5 == 5\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 =
// LOWERING-NEXT:             b"{{.*}}tests/fixtures/assert_compile_time_true.c\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                 as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:                 as *mut u8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 6;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"main\0".as_ptr() as *mut u8;
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
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"after\n\0".as_ptr() as *mut u8;
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
// REWRITES-NEXT:     unsafe { printf(c"before\n".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = 5 == {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             __assert_fail(
// REWRITES-NEXT:                 c"5 == 5".as_ptr(),
// REWRITES-NEXT:                 c"{{.*}}tests/fixtures/assert_compile_time_true.c".as_ptr(),
// REWRITES-NEXT:                 6 as i32,
// REWRITES-NEXT:                 c"main".as_ptr(),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf(c"after\n".as_ptr()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
