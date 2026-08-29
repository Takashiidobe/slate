#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  int *q = NULL;
  if (flag) {
    p = malloc(sizeof(int));
    q = malloc(sizeof(int));
  }
  if (p == q) {
    printf("same\n");
  } else {
    printf("diff\n");
  }
  if (p) {
    *p = 1;
    printf("%d\n", *p);
    free(p);
  }
  if (q) {
    *q = 2;
    printf("%d\n", *q);
    free(q);
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
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
// LOWERING-NEXT: fn compute(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut flag: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut q: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     flag = arg0;
// LOWERING-NEXT:     let _v0: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     q = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v2: i32 = flag;
// LOWERING-NEXT:         let _v3: bool = _v2 != 0;
// LOWERING-NEXT:         if _v3 {
// LOWERING-NEXT:             let _v4: u64 = 4;
// LOWERING-NEXT:             let _v5: *mut core::ffi::c_void = unsafe { malloc(_v4 as usize) };
// LOWERING-NEXT:             let _v6: *mut i32 = _v5 as *mut i32;
// LOWERING-NEXT:             p = _v6;
// LOWERING-NEXT:             let _v7: u64 = 4;
// LOWERING-NEXT:             let _v8: *mut core::ffi::c_void = unsafe { malloc(_v7 as usize) };
// LOWERING-NEXT:             let _v9: *mut i32 = _v8 as *mut i32;
// LOWERING-NEXT:             q = _v9;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v10: *mut i32 = p;
// LOWERING-NEXT:         let _v11: *mut i32 = q;
// LOWERING-NEXT:         let _v12: bool = _v10 == _v11;
// LOWERING-NEXT:         if _v12 {
// LOWERING-NEXT:             let _v13: *mut i8 = b"same\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v14: i32 = unsafe { printf(_v13 as *const i8) };
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let _v15: *mut i8 = b"diff\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v16: i32 = unsafe { printf(_v15 as *const i8) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v17: *mut i32 = p;
// LOWERING-NEXT:         let _v18: bool = _v17 != std::ptr::null_mut();
// LOWERING-NEXT:         if _v18 {
// LOWERING-NEXT:             let _v19: i32 = 1;
// LOWERING-NEXT:             let _v20: *mut i32 = p;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *_v20 = _v19;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v21: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v22: *mut i32 = p;
// LOWERING-NEXT:             let _v23: i32 = unsafe { *_v22 };
// LOWERING-NEXT:             let _v24: i32 = unsafe { printf(_v21 as *const i8, _v23) };
// LOWERING-NEXT:             let _v25: *mut i32 = p;
// LOWERING-NEXT:             let _v26: *mut core::ffi::c_void = _v25 as *mut core::ffi::c_void;
// LOWERING-NEXT:             unsafe { free(_v26 as *mut core::ffi::c_void) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v27: *mut i32 = q;
// LOWERING-NEXT:         let _v28: bool = _v27 != std::ptr::null_mut();
// LOWERING-NEXT:         if _v28 {
// LOWERING-NEXT:             let _v29: i32 = 2;
// LOWERING-NEXT:             let _v30: *mut i32 = q;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *_v30 = _v29;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v31: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v32: *mut i32 = q;
// LOWERING-NEXT:             let _v33: i32 = unsafe { *_v32 };
// LOWERING-NEXT:             let _v34: i32 = unsafe { printf(_v31 as *const i8, _v33) };
// LOWERING-NEXT:             let _v35: *mut i32 = q;
// LOWERING-NEXT:             let _v36: *mut core::ffi::c_void = _v35 as *mut core::ffi::c_void;
// LOWERING-NEXT:             unsafe { free(_v36 as *mut core::ffi::c_void) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v37: i32 = 0;
// LOWERING-NEXT:     __retval = _v37;
// LOWERING-NEXT:     let _v38: i32 = __retval;
// LOWERING-NEXT:     return _v38;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i32 = compute(_v1);
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     let _v4: i32 = compute(_v3);
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v6 as i32);
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
// REWRITES-NEXT: fn compute(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut flag: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut q: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: p = std::ptr::null_mut();
// REWRITES-NEXT: q = std::ptr::null_mut();
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v3: bool = flag != 0;
// REWRITES-NEXT:         if _v3 {
// REWRITES-NEXT:                     let _v4: u64 = 4;
// REWRITES-NEXT:                     let _v5: *mut core::ffi::c_void = unsafe { malloc(_v4 as usize) };
// REWRITES-NEXT:                     p = _v5 as *mut i32;
// REWRITES-NEXT:                     let _v7: u64 = 4;
// REWRITES-NEXT:                     let _v8: *mut core::ffi::c_void = unsafe { malloc(_v7 as usize) };
// REWRITES-NEXT:                     q = _v8 as *mut i32;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v12: bool = p == q;
// REWRITES-NEXT:         if _v12 {
// REWRITES-NEXT:                     let _v13: *mut i8 = b"same\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v14: i32 = unsafe { printf(_v13 as *const i8) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let _v15: *mut i8 = b"diff\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v16: i32 = unsafe { printf(_v15 as *const i8) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v18: bool = p != std::ptr::null_mut();
// REWRITES-NEXT:         if _v18 {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *p = 1;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v21: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v24: i32 = unsafe { printf(_v21 as *const i8, unsafe { *p }) };
// REWRITES-NEXT:                     unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v28: bool = q != std::ptr::null_mut();
// REWRITES-NEXT:         if _v28 {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *q = 2;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v31: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v34: i32 = unsafe { printf(_v31 as *const i8, unsafe { *q }) };
// REWRITES-NEXT:                     unsafe { free((q as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: let _v2: i32 = compute(_v1);
// REWRITES-NEXT: let _v3: i32 = 0;
// REWRITES-NEXT: let _v4: i32 = compute(_v3);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
