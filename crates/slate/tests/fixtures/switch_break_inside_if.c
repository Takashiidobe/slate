#include <stdio.h>

int classify(int x, int extra) {
  int result = -1;
  switch (x) {
  case 0:
    if (extra) {
      result = 10;
      break;
    }
    result = 11;
    break;
  case 1:
    result = 20;
    break;
  default:
    result = -2;
    break;
  }
  return result;
}

int main(void) {
  printf("%d\n", classify(0, 1));
  printf("%d\n", classify(0, 0));
  printf("%d\n", classify(1, 0));
  printf("%d\n", classify(5, 0));
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
// LOWERING-NEXT: fn classify(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut extra: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     extra = arg1;
// LOWERING-NEXT:     let _v0: i32 = -1;
// LOWERING-NEXT:     result = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: i32 = x;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = _v1;
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { 0 => 0, 1 => 1, _ => 2 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let _v2: i32 = extra;
// LOWERING-NEXT:                             let _v3: bool = _v2 != 0;
// LOWERING-NEXT:                             if _v3 {
// LOWERING-NEXT:                                 let _v4: i32 = 10;
// LOWERING-NEXT:                                 result = _v4;
// LOWERING-NEXT:                                 break '__switch0;
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let _v5: i32 = 11;
// LOWERING-NEXT:                         result = _v5;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let _v6: i32 = 20;
// LOWERING-NEXT:                         result = _v6;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let _v7: i32 = -2;
// LOWERING-NEXT:                         result = _v7;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v8: i32 = result;
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v9: i32 = __retval;
// LOWERING-NEXT:     return _v9;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     let _v3: i32 = 1;
// LOWERING-NEXT:     let _v4: i32 = classify(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     let _v8: i32 = 0;
// LOWERING-NEXT:     let _v9: i32 = classify(_v7, _v8);
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = 1;
// LOWERING-NEXT:     let _v13: i32 = 0;
// LOWERING-NEXT:     let _v14: i32 = classify(_v12, _v13);
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v11 as *const i8, _v14) };
// LOWERING-NEXT:     let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: i32 = 5;
// LOWERING-NEXT:     let _v18: i32 = 0;
// LOWERING-NEXT:     let _v19: i32 = classify(_v17, _v18);
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v16 as *const i8, _v19) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     __retval = _v21;
// LOWERING-NEXT:     let _v22: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v22 as i32);
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
// REWRITES-NEXT: fn classify(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut extra: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: result = -1;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = x;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { 0 => 0, 1 => 1, _ => 2 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             {
// REWRITES-NEXT:                                                                                     let _v3: bool = extra != 0;
// REWRITES-NEXT:                                                                                     if _v3 {
// REWRITES-NEXT:                                                                                                                 result = 10;
// REWRITES-NEXT:                                                                                                                 break '__switch0;
// REWRITES-NEXT:                                                                                     }
// REWRITES-NEXT:                                                             }
// REWRITES-NEXT:                                                             result = 11;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             result = 20;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         2 => {
// REWRITES-NEXT:                                                             result = -2;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = result;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 0;
// REWRITES-NEXT: let _v3: i32 = 1;
// REWRITES-NEXT: let _v4: i32 = classify(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = 0;
// REWRITES-NEXT: let _v8: i32 = 0;
// REWRITES-NEXT: let _v9: i32 = classify(_v7, _v8);
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = 1;
// REWRITES-NEXT: let _v13: i32 = 0;
// REWRITES-NEXT: let _v14: i32 = classify(_v12, _v13);
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v11 as *const i8, _v14) };
// REWRITES-NEXT: let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = 5;
// REWRITES-NEXT: let _v18: i32 = 0;
// REWRITES-NEXT: let _v19: i32 = classify(_v17, _v18);
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v16 as *const i8, _v19) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
