#include <stdio.h>

static int classify(int value) {
  int result = 0;
  switch (value) {
  case 1 ... 4:
    result += 10;
  case 5 ... 8:
    result += 20;
    break;
  case 10 ... 12:
    result += 40;
    break;
  default:
    result = 90;
  }
  return result;
}

static int classify_direct(int value) {
  switch (value) {
  case -2 ... 2:
    return 7;
  default:
    return 9;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", classify(1), classify(2),
         classify(4), classify(5), classify(7), classify(8), classify(9),
         classify(10), classify(11), classify(12), classify_direct(-1),
         classify_direct(3));
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
// LOWERING-NEXT: fn classify(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     value = arg1;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     result = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: i32 = value;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = _v1;
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { 1..=4 => 0, 5..=8 => 1, 10..=12 => 2, _ => 3 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let _v2: i32 = 10;
// LOWERING-NEXT:                         let _v3: i32 = result;
// LOWERING-NEXT:                         let _v4: i32 = _v3 + _v2;
// LOWERING-NEXT:                         result = _v4;
// LOWERING-NEXT:                         __switch_case0 = 1;
// LOWERING-NEXT:                         continue '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let _v5: i32 = 20;
// LOWERING-NEXT:                         let _v6: i32 = result;
// LOWERING-NEXT:                         let _v7: i32 = _v6 + _v5;
// LOWERING-NEXT:                         result = _v7;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let _v8: i32 = 40;
// LOWERING-NEXT:                         let _v9: i32 = result;
// LOWERING-NEXT:                         let _v10: i32 = _v9 + _v8;
// LOWERING-NEXT:                         result = _v10;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     3 => {
// LOWERING-NEXT:                         let _v11: i32 = 90;
// LOWERING-NEXT:                         result = _v11;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: i32 = result;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     return _v13;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn classify_direct(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = arg0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = value;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = _v0;
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { -2..=2 => 0, _ => 1 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let _v1: i32 = 7;
// LOWERING-NEXT:                         __retval = _v1;
// LOWERING-NEXT:                         let _v2: i32 = __retval;
// LOWERING-NEXT:                         return _v2;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let _v3: i32 = 9;
// LOWERING-NEXT:                         __retval = _v3;
// LOWERING-NEXT:                         let _v4: i32 = __retval;
// LOWERING-NEXT:                         return _v4;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 1;
// LOWERING-NEXT:     let _v3: i32 = classify(_v2);
// LOWERING-NEXT:     let _v4: i32 = 2;
// LOWERING-NEXT:     let _v5: i32 = classify(_v4);
// LOWERING-NEXT:     let _v6: i32 = 4;
// LOWERING-NEXT:     let _v7: i32 = classify(_v6);
// LOWERING-NEXT:     let _v8: i32 = 5;
// LOWERING-NEXT:     let _v9: i32 = classify(_v8);
// LOWERING-NEXT:     let _v10: i32 = 7;
// LOWERING-NEXT:     let _v11: i32 = classify(_v10);
// LOWERING-NEXT:     let _v12: i32 = 8;
// LOWERING-NEXT:     let _v13: i32 = classify(_v12);
// LOWERING-NEXT:     let _v14: i32 = 9;
// LOWERING-NEXT:     let _v15: i32 = classify(_v14);
// LOWERING-NEXT:     let _v16: i32 = 10;
// LOWERING-NEXT:     let _v17: i32 = classify(_v16);
// LOWERING-NEXT:     let _v18: i32 = 11;
// LOWERING-NEXT:     let _v19: i32 = classify(_v18);
// LOWERING-NEXT:     let _v20: i32 = 12;
// LOWERING-NEXT:     let _v21: i32 = classify(_v20);
// LOWERING-NEXT:     let _v22: i32 = -1;
// LOWERING-NEXT:     let _v23: i32 = classify_direct(_v22);
// LOWERING-NEXT:     let _v24: i32 = 3;
// LOWERING-NEXT:     let _v25: i32 = classify_direct(_v24);
// LOWERING-NEXT:     let _v26: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7, _v9, _v11, _v13, _v15, _v17, _v19, _v21, _v23, _v25) };
// LOWERING-NEXT:     let _v27: i32 = 0;
// LOWERING-NEXT:     __retval = _v27;
// LOWERING-NEXT:     let _v28: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v28 as i32);
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
// REWRITES-NEXT: fn classify(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: result = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = value;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { 1..=4 => 0, 5..=8 => 1, 10..=12 => 2, _ => 3 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             let _v2: i32 = 10;
// REWRITES-NEXT:                                                             result = result + _v2;
// REWRITES-NEXT:                                                             __switch_case0 = 1;
// REWRITES-NEXT:                                                             continue '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             let _v5: i32 = 20;
// REWRITES-NEXT:                                                             result = result + _v5;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         2 => {
// REWRITES-NEXT:                                                             let _v8: i32 = 40;
// REWRITES-NEXT:                                                             result = result + _v8;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         3 => {
// REWRITES-NEXT:                                                             result = 90;
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
// REWRITES-NEXT: fn classify_direct(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = value;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { -2..=2 => 0, _ => 1 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             __retval = 7;
// REWRITES-NEXT:                                                             return __retval;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             __retval = 9;
// REWRITES-NEXT:                                                             return __retval;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 1;
// REWRITES-NEXT: let _v3: i32 = classify(_v2);
// REWRITES-NEXT: let _v4: i32 = 2;
// REWRITES-NEXT: let _v5: i32 = classify(_v4);
// REWRITES-NEXT: let _v6: i32 = 4;
// REWRITES-NEXT: let _v7: i32 = classify(_v6);
// REWRITES-NEXT: let _v8: i32 = 5;
// REWRITES-NEXT: let _v9: i32 = classify(_v8);
// REWRITES-NEXT: let _v10: i32 = 7;
// REWRITES-NEXT: let _v11: i32 = classify(_v10);
// REWRITES-NEXT: let _v12: i32 = 8;
// REWRITES-NEXT: let _v13: i32 = classify(_v12);
// REWRITES-NEXT: let _v14: i32 = 9;
// REWRITES-NEXT: let _v15: i32 = classify(_v14);
// REWRITES-NEXT: let _v16: i32 = 10;
// REWRITES-NEXT: let _v17: i32 = classify(_v16);
// REWRITES-NEXT: let _v18: i32 = 11;
// REWRITES-NEXT: let _v19: i32 = classify(_v18);
// REWRITES-NEXT: let _v20: i32 = 12;
// REWRITES-NEXT: let _v21: i32 = classify(_v20);
// REWRITES-NEXT: let _v22: i32 = -1;
// REWRITES-NEXT: let _v23: i32 = classify_direct(_v22);
// REWRITES-NEXT: let _v24: i32 = 3;
// REWRITES-NEXT: let _v25: i32 = classify_direct(_v24);
// REWRITES-NEXT: let _v26: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7, _v9, _v11, _v13, _v15, _v17, _v19, _v21, _v23, _v25) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
