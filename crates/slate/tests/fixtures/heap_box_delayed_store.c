#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p      = malloc(sizeof(int));
  int  marker = 7;
  marker      = marker + 1;
  *p          = 41;
  printf("%d %d\n", *p, marker);
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
// LOWERING-NEXT:     let mut marker: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 4;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = unsafe { malloc(_v1 as usize) };
// LOWERING-NEXT:     let _v3: *mut i32 = _v2 as *mut i32;
// LOWERING-NEXT:     p = _v3;
// LOWERING-NEXT:     let _v4: i32 = 7;
// LOWERING-NEXT:     marker = _v4;
// LOWERING-NEXT:     let _v5: i32 = marker;
// LOWERING-NEXT:     let _v6: i32 = 1;
// LOWERING-NEXT:     let _v7: i32 = _v5 + _v6;
// LOWERING-NEXT:     marker = _v7;
// LOWERING-NEXT:     let _v8: i32 = 41;
// LOWERING-NEXT:     let _v9: *mut i32 = p;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v9 = _v8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: *mut i32 = p;
// LOWERING-NEXT:     let _v12: i32 = unsafe { *_v11 };
// LOWERING-NEXT:     let _v13: i32 = marker;
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v10 as *const i8, _v12, _v13) };
// LOWERING-NEXT:     let _v15: *mut i32 = p;
// LOWERING-NEXT:     let _v16: *mut core::ffi::c_void = _v15 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v16 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v17: i32 = 0;
// LOWERING-NEXT:     __retval = _v17;
// LOWERING-NEXT:     let _v18: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v18 as i32);
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
// REWRITES-NEXT: let mut marker: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 4;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = unsafe { malloc(_v1 as usize) };
// REWRITES-NEXT: p = _v2 as *mut i32;
// REWRITES-NEXT: marker = 7;
// REWRITES-NEXT: let _v6: i32 = 1;
// REWRITES-NEXT: marker = marker + _v6;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *p = 41;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v10: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v10 as *const i8, unsafe { *p }, marker) };
// REWRITES-NEXT: unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
