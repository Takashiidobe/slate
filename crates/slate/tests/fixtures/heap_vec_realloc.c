#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 2);
  p[0]   = 1;
  p[1]   = 2;
  p      = realloc(p, sizeof(int) * 4);
  p[2]   = 3;
  p[3]   = 4;
  printf("%d\n", p[0] + p[1] + p[2] + p[3]);
  free(p);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
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
// LOWERING-NEXT:     let _v2: u64 = 2;
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
// LOWERING-NEXT:     let _v14: *mut i32 = p;
// LOWERING-NEXT:     let _v15: *mut core::ffi::c_void = _v14 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v16: u64 = 4;
// LOWERING-NEXT:     let _v17: u64 = 4;
// LOWERING-NEXT:     let _v18: u64 = _v16 * _v17;
// LOWERING-NEXT:     let _v19: *mut core::ffi::c_void = unsafe { realloc(_v15 as *mut core::ffi::c_void, _v18 as usize) };
// LOWERING-NEXT:     let _v20: *mut i32 = _v19 as *mut i32;
// LOWERING-NEXT:     p = _v20;
// LOWERING-NEXT:     let _v21: i32 = 3;
// LOWERING-NEXT:     let _v22: i64 = 2;
// LOWERING-NEXT:     let _v23: *mut i32 = p;
// LOWERING-NEXT:     let _v24: *mut i32 = unsafe { _v23.add(2) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v24 = _v21;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v25: i32 = 4;
// LOWERING-NEXT:     let _v26: i64 = 3;
// LOWERING-NEXT:     let _v27: *mut i32 = p;
// LOWERING-NEXT:     let _v28: *mut i32 = unsafe { _v27.add(3) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v28 = _v25;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v29: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v30: i64 = 0;
// LOWERING-NEXT:     let _v31: *mut i32 = p;
// LOWERING-NEXT:     let _v32: *mut i32 = unsafe { _v31.add(0) };
// LOWERING-NEXT:     let _v33: i32 = unsafe { *_v32 };
// LOWERING-NEXT:     let _v34: i64 = 1;
// LOWERING-NEXT:     let _v35: *mut i32 = p;
// LOWERING-NEXT:     let _v36: *mut i32 = unsafe { _v35.add(1) };
// LOWERING-NEXT:     let _v37: i32 = unsafe { *_v36 };
// LOWERING-NEXT:     let _v38: i32 = _v33 + _v37;
// LOWERING-NEXT:     let _v39: i64 = 2;
// LOWERING-NEXT:     let _v40: *mut i32 = p;
// LOWERING-NEXT:     let _v41: *mut i32 = unsafe { _v40.add(2) };
// LOWERING-NEXT:     let _v42: i32 = unsafe { *_v41 };
// LOWERING-NEXT:     let _v43: i32 = _v38 + _v42;
// LOWERING-NEXT:     let _v44: i64 = 3;
// LOWERING-NEXT:     let _v45: *mut i32 = p;
// LOWERING-NEXT:     let _v46: *mut i32 = unsafe { _v45.add(3) };
// LOWERING-NEXT:     let _v47: i32 = unsafe { *_v46 };
// LOWERING-NEXT:     let _v48: i32 = _v43 + _v47;
// LOWERING-NEXT:     let _v49: i32 = unsafe { printf(_v29 as *const i8, _v48) };
// LOWERING-NEXT:     let _v50: *mut i32 = p;
// LOWERING-NEXT:     let _v51: *mut core::ffi::c_void = _v50 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v51 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v52: i32 = 0;
// LOWERING-NEXT:     __retval = _v52;
// LOWERING-NEXT:     let _v53: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v53 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 4;
// REWRITES-NEXT: let _v2: u64 = 2;
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
// REWRITES-NEXT: let _v16: u64 = 4;
// REWRITES-NEXT: let _v17: u64 = 4;
// REWRITES-NEXT: let _v19: *mut core::ffi::c_void = unsafe { realloc((p as *mut core::ffi::c_void) as *mut core::ffi::c_void, (_v16 * _v17) as usize) };
// REWRITES-NEXT: p = _v19 as *mut i32;
// REWRITES-NEXT: let _v21: i32 = 3;
// REWRITES-NEXT: let _v22: i64 = 2;
// REWRITES-NEXT: let _v23: *mut i32 = p;
// REWRITES-NEXT: let _v24: *mut i32 = unsafe { _v23.add(2) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v24 = _v21;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v25: i32 = 4;
// REWRITES-NEXT: let _v26: i64 = 3;
// REWRITES-NEXT: let _v27: *mut i32 = p;
// REWRITES-NEXT: let _v28: *mut i32 = unsafe { _v27.add(3) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v28 = _v25;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v29: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v30: i64 = 0;
// REWRITES-NEXT: let _v31: *mut i32 = p;
// REWRITES-NEXT: let _v32: *mut i32 = unsafe { _v31.add(0) };
// REWRITES-NEXT: let _v33: i32 = unsafe { *_v32 };
// REWRITES-NEXT: let _v34: i64 = 1;
// REWRITES-NEXT: let _v35: *mut i32 = p;
// REWRITES-NEXT: let _v36: *mut i32 = unsafe { _v35.add(1) };
// REWRITES-NEXT: let _v38: i32 = _v33 + unsafe { *_v36 };
// REWRITES-NEXT: let _v39: i64 = 2;
// REWRITES-NEXT: let _v40: *mut i32 = p;
// REWRITES-NEXT: let _v41: *mut i32 = unsafe { _v40.add(2) };
// REWRITES-NEXT: let _v43: i32 = _v38 + unsafe { *_v41 };
// REWRITES-NEXT: let _v44: i64 = 3;
// REWRITES-NEXT: let _v45: *mut i32 = p;
// REWRITES-NEXT: let _v46: *mut i32 = unsafe { _v45.add(3) };
// REWRITES-NEXT: let _v49: i32 = unsafe { printf(_v29 as *const i8, _v43 + unsafe { *_v46 }) };
// REWRITES-NEXT: unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
