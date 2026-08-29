#include <stdio.h>

int main(void) {
  int i     = 0;
  int steps = 0;
  while (i < 6) {
    int x = i % 3;
    switch (x) {
    case 0:
    case 1:
    case 2:
      i++;
      steps++;
      continue;
    default:
      break;
    }
    printf("unreachable %d\n", i);
    i++;
    steps++;
  }
  printf("steps=%d\n", steps);
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
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     let mut steps: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     i = _v1;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     steps = _v2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let _v3: i32 = i;
// LOWERING-NEXT:             let _v4: i32 = 6;
// LOWERING-NEXT:             let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:             if !_v5 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut x: i32 = 0;
// LOWERING-NEXT:                     let _v6: i32 = i;
// LOWERING-NEXT:                     let _v7: i32 = 3;
// LOWERING-NEXT:                     let _v8: i32 = _v6 % _v7;
// LOWERING-NEXT:                     x = _v8;
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let _v9: i32 = x;
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let __switch_value1 = _v9;
// LOWERING-NEXT:                             let mut __switch_case1: i32 = match __switch_value1 { 0 => 0, 1 => 1, 2 => 2, _ => 3 };
// LOWERING-NEXT:                             '__switch1: loop {
// LOWERING-NEXT:                                 match __switch_case1 {
// LOWERING-NEXT:                                     0 => {
// LOWERING-NEXT:                                         __switch_case1 = 1;
// LOWERING-NEXT:                                         continue '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     1 => {
// LOWERING-NEXT:                                         __switch_case1 = 2;
// LOWERING-NEXT:                                         continue '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     2 => {
// LOWERING-NEXT:                                         let _v10: i32 = i;
// LOWERING-NEXT:                                         let _v11: i32 = _v10 + 1;
// LOWERING-NEXT:                                         i = _v11;
// LOWERING-NEXT:                                         let _v12: i32 = steps;
// LOWERING-NEXT:                                         let _v13: i32 = _v12 + 1;
// LOWERING-NEXT:                                         steps = _v13;
// LOWERING-NEXT:                                         break '__continue0;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     3 => {
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     _ => {
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let _v14: *mut i8 = b"unreachable %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                     let _v15: i32 = i;
// LOWERING-NEXT:                     let _v16: i32 = unsafe { printf(_v14 as *const i8, _v15) };
// LOWERING-NEXT:                     let _v17: i32 = i;
// LOWERING-NEXT:                     let _v18: i32 = _v17 + 1;
// LOWERING-NEXT:                     i = _v18;
// LOWERING-NEXT:                     let _v19: i32 = steps;
// LOWERING-NEXT:                     let _v20: i32 = _v19 + 1;
// LOWERING-NEXT:                     steps = _v20;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v21: *mut i8 = b"steps=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: i32 = steps;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v21 as *const i8, _v22) };
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
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut i: i32 = 0;
// REWRITES-NEXT: let mut steps: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: i = 0;
// REWRITES-NEXT: steps = 0;
// REWRITES-NEXT: '__loop0: loop {
// REWRITES-NEXT:         let _v4: i32 = 6;
// REWRITES-NEXT:         if !(i < _v4) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         '__continue0: {
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut x: i32 = 0;
// REWRITES-NEXT:                                     let _v7: i32 = 3;
// REWRITES-NEXT:                                     x = i % _v7;
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let __switch_value1 = x;
// REWRITES-NEXT:                                                                                 let mut __switch_case1: i32 = match __switch_value1 { 0 => 0, 1 => 1, 2 => 2, _ => 3 };
// REWRITES-NEXT:                                                                                 '__switch1: loop {
// REWRITES-NEXT:                                                                                                             match __switch_case1 {
// REWRITES-NEXT:                                                                                                                 0 => {
// REWRITES-NEXT:                                                                                                                                                 __switch_case1 = 1;
// REWRITES-NEXT:                                                                                                                                                 continue '__switch1;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                                                 1 => {
// REWRITES-NEXT:                                                                                                                                                 __switch_case1 = 2;
// REWRITES-NEXT:                                                                                                                                                 continue '__switch1;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                                                 2 => {
// REWRITES-NEXT:                                                                                                                                                 i = i + 1;
// REWRITES-NEXT:                                                                                                                                                 steps = steps + 1;
// REWRITES-NEXT:                                                                                                                                                 break '__continue0;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                                                 3 => {
// REWRITES-NEXT:                                                                                                                                                 break '__switch1;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                                                 _ => {
// REWRITES-NEXT:                                                                                                                                                 break '__switch1;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     let _v14: *mut i8 = b"unreachable %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                     let _v16: i32 = unsafe { printf(_v14 as *const i8, i) };
// REWRITES-NEXT:                                     i = i + 1;
// REWRITES-NEXT:                                     steps = steps + 1;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v21: *mut i8 = b"steps=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v21 as *const i8, steps) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
