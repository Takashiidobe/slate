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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn __assert_fail(_0: *const i8, _1: *const i8, _2: i32, _3: *const i8) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { printf(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: i32 = 5;
// LOWERING-NEXT:     let _v4: i32 = 5;
// LOWERING-NEXT:     let _v5: bool = _v3 == _v4;
// LOWERING-NEXT:     let _v6: bool = if _v5 {
// LOWERING-NEXT:         let _v7: bool = true;
// LOWERING-NEXT:         _v7
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v8: *mut i8 = b"5 == 5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v9: *mut i8 = b"{{.*}}tests/fixtures/assert_compile_time_true.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v10: i32 = 6;
// LOWERING-NEXT:         let _v11: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         unsafe { __assert_fail(_v8 as *const i8, _v9 as *const i8, _v10 as i32, _v11 as *const i8) };
// LOWERING-NEXT:         let _v12: i32 = 0;
// LOWERING-NEXT:         let _v13: bool = _v12 != 0;
// LOWERING-NEXT:         _v13
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v14: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v14 as *const i8) };
// LOWERING-NEXT:     let _v16: i32 = 0;
// LOWERING-NEXT:     __retval = _v16;
// LOWERING-NEXT:     let _v17: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v17 as i32);
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
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"before\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { printf(_v1 as *const i8) };
// REWRITES-NEXT: let _v3: i32 = 5;
// REWRITES-NEXT: let _v4: i32 = 5;
// REWRITES-NEXT: let _v5: bool = _v3 == _v4;
// REWRITES-NEXT: let _v6: bool = if _v5 {
// REWRITES-NEXT:         let _v7: bool = true;
// REWRITES-NEXT:     _v7
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v8: *mut i8 = b"5 == 5\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v9: *mut i8 = b"{{.*}}tests/fixtures/assert_compile_time_true.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v10: i32 = 6;
// REWRITES-NEXT:         let _v11: *mut i8 = b"main\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         unsafe { __assert_fail(_v8 as *const i8, _v9 as *const i8, _v10 as i32, _v11 as *const i8) };
// REWRITES-NEXT:         let _v12: i32 = 0;
// REWRITES-NEXT:         let _v13: bool = _v12 != 0;
// REWRITES-NEXT:     _v13
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v14: *mut i8 = b"after\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v14 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
