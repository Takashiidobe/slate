#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 3);
  p[0]   = 1;
  p[1]   = 2;
  p[2]   = 3;
  printf("%d\n", p[0] + p[1] + p[2]);
  free(p);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 4;
// LOWERING-NEXT:     let _v2: u64 = 3;
// LOWERING-NEXT:     let _v3: u64 = _v1 * _v2;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = unsafe { malloc(_v3 as usize) };
// LOWERING-NEXT:     let _v5: *mut i32 = _v4 as *mut i32;
// LOWERING-NEXT:     p = _v5;
// LOWERING-NEXT:     let _v6: i32 = 1;
// LOWERING-NEXT:     let _v7: i64 = 0;
// LOWERING-NEXT:     let _v8: *mut i32 = p;
// LOWERING-NEXT:     let _v9: *mut i32 = unsafe { _v8.add(0) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v9 = _v6;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v10: i32 = 2;
// LOWERING-NEXT:     let _v11: i64 = 1;
// LOWERING-NEXT:     let _v12: *mut i32 = p;
// LOWERING-NEXT:     let _v13: *mut i32 = unsafe { _v12.add(1) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v13 = _v10;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i32 = 3;
// LOWERING-NEXT:     let _v15: i64 = 2;
// LOWERING-NEXT:     let _v16: *mut i32 = p;
// LOWERING-NEXT:     let _v17: *mut i32 = unsafe { _v16.add(2) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v17 = _v14;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: i64 = 0;
// LOWERING-NEXT:     let _v20: *mut i32 = p;
// LOWERING-NEXT:     let _v21: *mut i32 = unsafe { _v20.add(0) };
// LOWERING-NEXT:     let _v22: i32 = unsafe { *_v21 };
// LOWERING-NEXT:     let _v23: i64 = 1;
// LOWERING-NEXT:     let _v24: *mut i32 = p;
// LOWERING-NEXT:     let _v25: *mut i32 = unsafe { _v24.add(1) };
// LOWERING-NEXT:     let _v26: i32 = unsafe { *_v25 };
// LOWERING-NEXT:     let _v27: i32 = _v22 + _v26;
// LOWERING-NEXT:     let _v28: i64 = 2;
// LOWERING-NEXT:     let _v29: *mut i32 = p;
// LOWERING-NEXT:     let _v30: *mut i32 = unsafe { _v29.add(2) };
// LOWERING-NEXT:     let _v31: i32 = unsafe { *_v30 };
// LOWERING-NEXT:     let _v32: i32 = _v27 + _v31;
// LOWERING-NEXT:     let _v33: i32 = unsafe { printf(_v18 as *const i8, _v32) };
// LOWERING-NEXT:     let _v34: *mut i32 = p;
// LOWERING-NEXT:     let _v35: *mut core::ffi::c_void = _v34 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v35 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v36: i32 = 0;
// LOWERING-NEXT:     __retval = _v36;
// LOWERING-NEXT:     let _v37: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v37 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 4;
// REWRITES-NEXT: let _v2: u64 = 3;
// REWRITES-NEXT: let _v4: *mut core::ffi::c_void = unsafe { malloc((_v1 * _v2) as usize) };
// REWRITES-NEXT: p = _v4 as *mut i32;
// REWRITES-NEXT: let _v6: i32 = 1;
// REWRITES-NEXT: let _v7: i64 = 0;
// REWRITES-NEXT: let _v8: *mut i32 = p;
// REWRITES-NEXT: let _v9: *mut i32 = unsafe { _v8.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v9 = _v6;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v10: i32 = 2;
// REWRITES-NEXT: let _v11: i64 = 1;
// REWRITES-NEXT: let _v12: *mut i32 = p;
// REWRITES-NEXT: let _v13: *mut i32 = unsafe { _v12.add(1) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v13 = _v10;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v14: i32 = 3;
// REWRITES-NEXT: let _v15: i64 = 2;
// REWRITES-NEXT: let _v16: *mut i32 = p;
// REWRITES-NEXT: let _v17: *mut i32 = unsafe { _v16.add(2) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v17 = _v14;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i64 = 0;
// REWRITES-NEXT: let _v20: *mut i32 = p;
// REWRITES-NEXT: let _v21: *mut i32 = unsafe { _v20.add(0) };
// REWRITES-NEXT: let _v22: i32 = unsafe { *_v21 };
// REWRITES-NEXT: let _v23: i64 = 1;
// REWRITES-NEXT: let _v24: *mut i32 = p;
// REWRITES-NEXT: let _v25: *mut i32 = unsafe { _v24.add(1) };
// REWRITES-NEXT: let _v27: i32 = _v22 + unsafe { *_v25 };
// REWRITES-NEXT: let _v28: i64 = 2;
// REWRITES-NEXT: let _v29: *mut i32 = p;
// REWRITES-NEXT: let _v30: *mut i32 = unsafe { _v29.add(2) };
// REWRITES-NEXT: let _v33: i32 = unsafe { printf(_v18 as *const i8, _v27 + unsafe { *_v30 }) };
// REWRITES-NEXT: unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
