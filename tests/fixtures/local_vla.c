#include <stdio.h>

static int sum_vla(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

int main(void) {
  int result;
  {
    int length = 4;
    int values[length];
    for (int index = 0; index < length; ++index) {
      values[index] = index + 3;
    }
    result = sum_vla(length, &values);
  }
  printf("%d\n", result + 1);
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
// LOWERING-NEXT: fn sum_vla(arg0: i32, arg1: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     length = arg0;
// LOWERING-NEXT:     values = arg1;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         index = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = index;
// LOWERING-NEXT:             let _v3: i32 = length;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = index;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: *mut i32 = values;
// LOWERING-NEXT:                 let _v8: *mut i32 = unsafe { _v7.offset(_v6 as isize) };
// LOWERING-NEXT:                 let _v9: i32 = unsafe { *_v8 };
// LOWERING-NEXT:                 let _v10: i32 = total;
// LOWERING-NEXT:                 let _v11: i32 = _v10 + _v9;
// LOWERING-NEXT:                 total = _v11;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v12: i32 = index;
// LOWERING-NEXT:             let _v13: i32 = _v12 + 1;
// LOWERING-NEXT:             index = _v13;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i32 = total;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut length: i32 = 0;
// LOWERING-NEXT:         let mut saved_stack: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:         let _v1: i32 = 4;
// LOWERING-NEXT:         length = _v1;
// LOWERING-NEXT:         let _v2: i32 = length;
// LOWERING-NEXT:         let _v3: u64 = _v2 as u64;
// LOWERING-NEXT:         let _v4: *mut u8 = 0usize as *mut u8;
// LOWERING-NEXT:         saved_stack = _v4;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let mut values: Vec<i32> = vec![0; _v3 as usize];
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut index: i32 = 0;
// LOWERING-NEXT:                 let _v5: i32 = 0;
// LOWERING-NEXT:                 index = _v5;
// LOWERING-NEXT:                 loop {
// LOWERING-NEXT:                     let _v6: i32 = index;
// LOWERING-NEXT:                     let _v7: i32 = length;
// LOWERING-NEXT:                     let _v8: bool = _v6 < _v7;
// LOWERING-NEXT:                     if !_v8 {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let _v9: i32 = index;
// LOWERING-NEXT:                         let _v10: i32 = 3;
// LOWERING-NEXT:                         let _v11: i32 = _v9 + _v10;
// LOWERING-NEXT:                         let _v12: i32 = index;
// LOWERING-NEXT:                         let _v13: i64 = _v12 as i64;
// LOWERING-NEXT:                         let _v14: *mut i32 = unsafe { values.as_mut_ptr().offset(_v13 as isize) };
// LOWERING-NEXT:                         unsafe {
// LOWERING-NEXT:                             *_v14 = _v11;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let _v15: i32 = index;
// LOWERING-NEXT:                     let _v16: i32 = _v15 + 1;
// LOWERING-NEXT:                     index = _v16;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v17: i32 = length;
// LOWERING-NEXT:             let _v18: i32 = sum_vla(_v17, values.as_mut_ptr());
// LOWERING-NEXT:             result = _v18;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v19: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: i32 = result;
// LOWERING-NEXT:     let _v21: i32 = 1;
// LOWERING-NEXT:     let _v22: i32 = _v20 + _v21;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v19 as *const i8, _v22) };
// LOWERING-NEXT:     let _v24: i32 = 0;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v25 as i32);
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
// REWRITES-NEXT: fn sum_vla(arg0: i32, arg1: *mut i32) -> i32 {
// REWRITES-NEXT: let __arg1_view = unsafe { std::slice::from_raw_parts(arg1 as *const i32, arg0 as usize) };
// REWRITES-NEXT: let mut length: i32 = arg0;
// REWRITES-NEXT: let mut values: *mut i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index: i32 = 0;
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(index < length) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v7: *mut i32 = values;
// REWRITES-NEXT:                                     let _v8: *mut i32 = unsafe { _v7.offset((index as i64) as isize) };
// REWRITES-NEXT:                                     total = total + unsafe { __arg1_view[(index as usize)] };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut length: i32 = 0;
// REWRITES-NEXT:         let mut saved_stack: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT:         length = 4;
// REWRITES-NEXT:         let _v3: u64 = length as u64;
// REWRITES-NEXT:         saved_stack = 0usize as *mut u8;
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let mut values: Vec<i32> = vec![0; _v3 as usize];
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut index: i32 = 0;
// REWRITES-NEXT:                                     index = 0;
// REWRITES-NEXT:                                     loop {
// REWRITES-NEXT:                                                         if !(index < length) {
// REWRITES-NEXT:                                                                                 break;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let _v10: i32 = 3;
// REWRITES-NEXT:                                                                                 let _v11: i32 = index + _v10;
// REWRITES-NEXT:                                                                                 let _v14: *mut i32 = unsafe { values.as_mut_ptr().offset((index as i64) as isize) };
// REWRITES-NEXT:                                                                                 unsafe {
// REWRITES-NEXT:                                                                                                             *_v14 = _v11;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         index = index + 1;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     result = sum_vla(length, values.as_mut_ptr());
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v19: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v21: i32 = 1;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v19 as *const i8, result + _v21) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
