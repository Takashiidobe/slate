#include <stdio.h>

int map(int x) {
  int out = 0;
  switch (x) {
  case -3:
    out = 13;
    break;
  case 100:
    out = 1000;
    break;
  default:
    out = -1;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", map(-3), map(100), map(0));
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
// LOWERING-NEXT: fn map(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut out: i32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     out = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: i32 = x;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = _v1;
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { -3 => 0, 100 => 1, _ => 2 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let _v2: i32 = 13;
// LOWERING-NEXT:                         out = _v2;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let _v3: i32 = 1000;
// LOWERING-NEXT:                         out = _v3;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let _v4: i32 = -1;
// LOWERING-NEXT:                         out = _v4;
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: i32 = out;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = -3;
// LOWERING-NEXT:     let _v3: i32 = map(_v2);
// LOWERING-NEXT:     let _v4: i32 = 100;
// LOWERING-NEXT:     let _v5: i32 = map(_v4);
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     let _v7: i32 = map(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
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
// REWRITES-NEXT: fn map(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut out: i32 = 0;
// REWRITES-NEXT: out = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = x;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { -3 => 0, 100 => 1, _ => 2 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             out = 13;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             out = 1000;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         2 => {
// REWRITES-NEXT:                                                             out = -1;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = out;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = -3;
// REWRITES-NEXT: let _v3: i32 = map(_v2);
// REWRITES-NEXT: let _v4: i32 = 100;
// REWRITES-NEXT: let _v5: i32 = map(_v4);
// REWRITES-NEXT: let _v6: i32 = 0;
// REWRITES-NEXT: let _v7: i32 = map(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
