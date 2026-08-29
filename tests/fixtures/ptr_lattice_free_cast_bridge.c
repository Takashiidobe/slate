#include <stdio.h>
#include <stdlib.h>

static void just_free(int *p) { free(p); }

int main(void) {
  int *x = malloc(sizeof(int));
  *x = 4;
  printf("%d\n", *x);
  just_free(x);
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
// LOWERING-NEXT: fn just_free(arg0: *mut i32) {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     let _v0: *mut i32 = p;
// LOWERING-NEXT:     let _v1: *mut core::ffi::c_void = _v0 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v1 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 4;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = unsafe { malloc(_v1 as usize) };
// LOWERING-NEXT:     let _v3: *mut i32 = _v2 as *mut i32;
// LOWERING-NEXT:     x = _v3;
// LOWERING-NEXT:     let _v4: i32 = 4;
// LOWERING-NEXT:     let _v5: *mut i32 = x;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v5 = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: *mut i32 = x;
// LOWERING-NEXT:     let _v8: i32 = unsafe { *_v7 };
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v6 as *const i8, _v8) };
// LOWERING-NEXT:     let _v10: *mut i32 = x;
// LOWERING-NEXT:     just_free(_v10);
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
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
// REWRITES-NEXT: fn just_free(arg0: *mut i32) {
// REWRITES-NEXT: let mut p: *mut i32 = arg0;
// REWRITES-NEXT: unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 4;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = unsafe { malloc(_v1 as usize) };
// REWRITES-NEXT: x = _v2 as *mut i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *x = 4;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v6 as *const i8, unsafe { *x }) };
// REWRITES-NEXT: just_free(x);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
