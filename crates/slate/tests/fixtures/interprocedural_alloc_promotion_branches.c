#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int *a(void) { return alloc(); }

int *b() {
  if (1)
    return a();
  return alloc();
}

int *c(void) { return b(); }

int main(void) {
  int *x = NULL;
  x      = c();
  x[0]   = 10;
  printf("%d\n", x[0]);
  free(x);
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
// LOWERING-NEXT: fn alloc() -> *mut i32 {
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: u64 = 4;
// LOWERING-NEXT:     let _v1: u64 = 10;
// LOWERING-NEXT:     let _v2: u64 = _v0 * _v1;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = unsafe { malloc(_v2 as usize) };
// LOWERING-NEXT:     let _v4: *mut i32 = _v3 as *mut i32;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: *mut i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn a() -> *mut i32 {
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: *mut i32 = alloc();
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn b() -> *mut i32 {
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = 1;
// LOWERING-NEXT:         let _v1: bool = _v0 != 0;
// LOWERING-NEXT:         if _v1 {
// LOWERING-NEXT:             let _v2: *mut i32 = a();
// LOWERING-NEXT:             __retval = _v2;
// LOWERING-NEXT:             let _v3: *mut i32 = __retval;
// LOWERING-NEXT:             return _v3;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: *mut i32 = alloc();
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: *mut i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c() -> *mut i32 {
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: *mut i32 = b();
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: *mut i32 = c();
// LOWERING-NEXT:     x = _v2;
// LOWERING-NEXT:     let _v3: i32 = 10;
// LOWERING-NEXT:     let _v4: i64 = 0;
// LOWERING-NEXT:     let _v5: *mut i32 = x;
// LOWERING-NEXT:     let _v6: *mut i32 = unsafe { _v5.add(0) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v6 = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v7: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i64 = 0;
// LOWERING-NEXT:     let _v9: *mut i32 = x;
// LOWERING-NEXT:     let _v10: *mut i32 = unsafe { _v9.add(0) };
// LOWERING-NEXT:     let _v11: i32 = unsafe { *_v10 };
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v7 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: *mut i32 = x;
// LOWERING-NEXT:     let _v14: *mut core::ffi::c_void = _v13 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v14 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v15: i32 = 0;
// LOWERING-NEXT:     __retval = _v15;
// LOWERING-NEXT:     let _v16: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v16 as i32);
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
// REWRITES-NEXT: fn alloc() -> *mut i32 {
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: u64 = 4;
// REWRITES-NEXT: let _v1: u64 = 10;
// REWRITES-NEXT: let _v3: *mut core::ffi::c_void = unsafe { malloc((_v0 * _v1) as usize) };
// REWRITES-NEXT: __retval = _v3 as *mut i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn a() -> *mut i32 {
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = alloc();
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn b() -> *mut i32 {
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v0: i32 = 1;
// REWRITES-NEXT:         let _v1: bool = _v0 != 0;
// REWRITES-NEXT:         if _v1 {
// REWRITES-NEXT:                     __retval = a();
// REWRITES-NEXT:                     return __retval;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = alloc();
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c() -> *mut i32 {
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = b();
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = std::ptr::null_mut();
// REWRITES-NEXT: x = c();
// REWRITES-NEXT: let _v3: i32 = 10;
// REWRITES-NEXT: let _v4: i64 = 0;
// REWRITES-NEXT: let _v5: *mut i32 = x;
// REWRITES-NEXT: let _v6: *mut i32 = unsafe { _v5.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v6 = _v3;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v7: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i64 = 0;
// REWRITES-NEXT: let _v9: *mut i32 = x;
// REWRITES-NEXT: let _v10: *mut i32 = unsafe { _v9.add(0) };
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v7 as *const i8, unsafe { *_v10 }) };
// REWRITES-NEXT: unsafe { free((x as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
