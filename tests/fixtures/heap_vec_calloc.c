#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = calloc(4, sizeof(int));
  printf("%d\n", p[0] + p[3]);
  free(p);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn calloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
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
// LOWERING-NEXT:     let _v2: u64 = 4;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = unsafe { calloc(_v1 as usize, _v2 as usize) };
// LOWERING-NEXT:     let _v4: *mut i32 = _v3 as *mut i32;
// LOWERING-NEXT:     p = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i64 = 0;
// LOWERING-NEXT:     let _v7: *mut i32 = p;
// LOWERING-NEXT:     let _v8: *mut i32 = unsafe { _v7.add(0) };
// LOWERING-NEXT:     let _v9: i32 = unsafe { *_v8 };
// LOWERING-NEXT:     let _v10: i64 = 3;
// LOWERING-NEXT:     let _v11: *mut i32 = p;
// LOWERING-NEXT:     let _v12: *mut i32 = unsafe { _v11.add(3) };
// LOWERING-NEXT:     let _v13: i32 = unsafe { *_v12 };
// LOWERING-NEXT:     let _v14: i32 = _v9 + _v13;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v5 as *const i8, _v14) };
// LOWERING-NEXT:     let _v16: *mut i32 = p;
// LOWERING-NEXT:     let _v17: *mut core::ffi::c_void = _v16 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v17 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v18: i32 = 0;
// LOWERING-NEXT:     __retval = _v18;
// LOWERING-NEXT:     let _v19: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v19 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn calloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 4;
// REWRITES-NEXT: let _v2: u64 = 4;
// REWRITES-NEXT: let _v3: *mut core::ffi::c_void = unsafe { calloc(_v1 as usize, _v2 as usize) };
// REWRITES-NEXT: p = _v3 as *mut i32;
// REWRITES-NEXT: let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i64 = 0;
// REWRITES-NEXT: let _v7: *mut i32 = p;
// REWRITES-NEXT: let _v8: *mut i32 = unsafe { _v7.add(0) };
// REWRITES-NEXT: let _v9: i32 = unsafe { *_v8 };
// REWRITES-NEXT: let _v10: i64 = 3;
// REWRITES-NEXT: let _v11: *mut i32 = p;
// REWRITES-NEXT: let _v12: *mut i32 = unsafe { _v11.add(3) };
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v5 as *const i8, _v9 + unsafe { *_v12 }) };
// REWRITES-NEXT: unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
