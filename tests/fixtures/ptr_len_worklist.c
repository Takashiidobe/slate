#include <stdio.h>
#include <stdlib.h>

static void fill_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] = i * 3;
}

static void forward_fill(int *values, int len) {
  fill_values(values, len);
}

int main(void) {
  int len = 5;
  int *values = malloc(len * sizeof(int));
  forward_fill(values, len);
  printf("%d %d\n", values[1], values[4]);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill_values(arg2: *mut i32, arg3: i32) {
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     values = arg2;
// LOWERING-NEXT:     len = arg3;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v0: i32 = 0;
// LOWERING-NEXT:         i = _v0;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = i;
// LOWERING-NEXT:             let _v2: i32 = len;
// LOWERING-NEXT:             let _v3: bool = _v1 < _v2;
// LOWERING-NEXT:             if !_v3 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v4: i32 = i;
// LOWERING-NEXT:             let _v5: i32 = 3;
// LOWERING-NEXT:             let _v6: i32 = _v4 * _v5;
// LOWERING-NEXT:             let _v7: i32 = i;
// LOWERING-NEXT:             let _v8: i64 = _v7 as i64;
// LOWERING-NEXT:             let _v9: *mut i32 = values;
// LOWERING-NEXT:             let _v10: *mut i32 = unsafe { _v9.offset(_v8 as isize) };
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *_v10 = _v6;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v11: i32 = i;
// LOWERING-NEXT:             let _v12: i32 = _v11 + 1;
// LOWERING-NEXT:             i = _v12;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn forward_fill(arg0: *mut i32, arg1: i32) {
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     values = arg0;
// LOWERING-NEXT:     len = arg1;
// LOWERING-NEXT:     let _v0: *mut i32 = values;
// LOWERING-NEXT:     let _v1: i32 = len;
// LOWERING-NEXT:     fill_values(_v0, _v1);
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     len = _v1;
// LOWERING-NEXT:     let _v2: i32 = len;
// LOWERING-NEXT:     let _v3: u64 = _v2 as u64;
// LOWERING-NEXT:     let _v4: u64 = 4;
// LOWERING-NEXT:     let _v5: u64 = _v3 * _v4;
// LOWERING-NEXT:     let _v6: *mut core::ffi::c_void = unsafe { malloc(_v5 as usize) };
// LOWERING-NEXT:     let _v7: *mut i32 = _v6 as *mut i32;
// LOWERING-NEXT:     values = _v7;
// LOWERING-NEXT:     let _v8: *mut i32 = values;
// LOWERING-NEXT:     let _v9: i32 = len;
// LOWERING-NEXT:     forward_fill(_v8, _v9);
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i64 = 1;
// LOWERING-NEXT:     let _v12: *mut i32 = values;
// LOWERING-NEXT:     let _v13: *mut i32 = unsafe { _v12.add(1) };
// LOWERING-NEXT:     let _v14: i32 = unsafe { *_v13 };
// LOWERING-NEXT:     let _v15: i64 = 4;
// LOWERING-NEXT:     let _v16: *mut i32 = values;
// LOWERING-NEXT:     let _v17: *mut i32 = unsafe { _v16.add(4) };
// LOWERING-NEXT:     let _v18: i32 = unsafe { *_v17 };
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v10 as *const i8, _v14, _v18) };
// LOWERING-NEXT:     let _v20: i32 = 0;
// LOWERING-NEXT:     __retval = _v20;
// LOWERING-NEXT:     let _v21: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v21 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill_values(arg2: &mut [i32]) {
// REWRITES-NEXT: let mut values: *mut i32 = arg2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let mut len: i32 = arg2.len() as i32;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < len) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v5: i32 = 3;
// REWRITES-NEXT:                     let _v6: i32 = i * _v5;
// REWRITES-NEXT:                     let _v9: *mut i32 = values;
// REWRITES-NEXT:                     let _v10: *mut i32 = unsafe { _v9.offset((i as i64) as isize) };
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     *_v10 = _v6;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn forward_fill(arg0: &mut [i32]) {
// REWRITES-NEXT: let mut values: *mut i32 = arg0.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let mut len: i32 = arg0.len() as i32;
// REWRITES-NEXT: let _v0: *mut i32 = values;
// REWRITES-NEXT: let _v1: i32 = len;
// REWRITES-NEXT: fill_values(arg0);
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut len: i32 = 0;
// REWRITES-NEXT: let mut values: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: len = 5;
// REWRITES-NEXT: let _v4: u64 = 4;
// REWRITES-NEXT: let _v6: *mut core::ffi::c_void = unsafe { malloc(((len as u64) * _v4) as usize) };
// REWRITES-NEXT: values = _v6 as *mut i32;
// REWRITES-NEXT: let _v8: *mut i32 = values;
// REWRITES-NEXT: let _v9: i32 = len;
// REWRITES-NEXT: forward_fill(unsafe { std::slice::from_raw_parts_mut(_v8 as *mut i32, _v9 as usize) });
// REWRITES-NEXT: let _v10: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i64 = 1;
// REWRITES-NEXT: let _v12: *mut i32 = values;
// REWRITES-NEXT: let _v13: *mut i32 = unsafe { _v12.add(1) };
// REWRITES-NEXT: let _v14: i32 = unsafe { *_v13 };
// REWRITES-NEXT: let _v15: i64 = 4;
// REWRITES-NEXT: let _v16: *mut i32 = values;
// REWRITES-NEXT: let _v17: *mut i32 = unsafe { _v16.add(4) };
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v10 as *const i8, _v14, unsafe { *_v17 }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
