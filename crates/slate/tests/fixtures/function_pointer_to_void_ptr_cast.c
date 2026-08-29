#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

static void *store_fn(void *fn) { return fn; }

int main(void) {
  void    *slot = (void *)add_one;
  Callback cb   = (Callback)slot;

  void    *slot2 = store_fn((void *)add_one);
  Callback cb2   = (Callback)slot2;

  printf("%d %d\n", cb(41), cb2(99));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add_one(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn store_fn(arg0: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut r#fn: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     r#fn = arg0;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = r#fn;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut core::ffi::c_void = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut slot: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut cb: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let mut slot2: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut cb2: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(Some(add_one)) };
// LOWERING-NEXT:     slot = _v1;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = slot;
// LOWERING-NEXT:     let _v3: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>(_v2 as usize) };
// LOWERING-NEXT:     cb = _v3;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(Some(add_one)) };
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = store_fn(_v4 as *mut core::ffi::c_void);
// LOWERING-NEXT:     slot2 = _v5;
// LOWERING-NEXT:     let _v6: *mut core::ffi::c_void = slot2;
// LOWERING-NEXT:     let _v7: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>(_v6 as usize) };
// LOWERING-NEXT:     cb2 = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: Option<unsafe extern "C" fn(i32) -> i32> = cb;
// LOWERING-NEXT:     let _v10: i32 = 41;
// LOWERING-NEXT:     let _v11: i32 = unsafe { _v9.unwrap()(_v10) };
// LOWERING-NEXT:     let _v12: Option<unsafe extern "C" fn(i32) -> i32> = cb2;
// LOWERING-NEXT:     let _v13: i32 = 99;
// LOWERING-NEXT:     let _v14: i32 = unsafe { _v12.unwrap()(_v13) };
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v8 as *const i8, _v11, _v14) };
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
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add_one(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: __retval = x + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn store_fn(arg0: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// REWRITES-NEXT: let mut r#fn: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: __retval = r#fn;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut slot: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut cb: Option<unsafe extern "C" fn(i32) -> i32> = None;
// REWRITES-NEXT: let mut slot2: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut cb2: Option<unsafe extern "C" fn(i32) -> i32> = None;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: slot = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(Some(add_one)) };
// REWRITES-NEXT: let _v3: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>(slot as usize) };
// REWRITES-NEXT: cb = _v3;
// REWRITES-NEXT: let _v4: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(Some(add_one)) };
// REWRITES-NEXT: slot2 = store_fn(_v4 as *mut core::ffi::c_void);
// REWRITES-NEXT: let _v7: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>(slot2 as usize) };
// REWRITES-NEXT: cb2 = _v7;
// REWRITES-NEXT: let _v8: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = 41;
// REWRITES-NEXT: let _v11: i32 = unsafe { cb.unwrap()(_v10) };
// REWRITES-NEXT: let _v13: i32 = 99;
// REWRITES-NEXT: let _v14: i32 = unsafe { cb2.unwrap()(_v13) };
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v8 as *const i8, _v11, _v14) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
