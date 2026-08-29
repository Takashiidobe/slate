#include <stdio.h>

int classify(int n) {
  if (n < 0)
    goto neg;
  if (n == 0)
    goto zero;
  return 1;
neg:
  return -1;
zero:
  return 0;
}

int main() {
  printf("%d %d %d\n", classify(-5), classify(0), classify(42));
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
// LOWERING-NEXT: fn classify(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 n = arg0;
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v0: i32 = n;
// LOWERING-NEXT:                     let _v1: i32 = 0;
// LOWERING-NEXT:                     let _v2: bool = _v0 < _v1;
// LOWERING-NEXT:                     if _v2 {
// LOWERING-NEXT:                         __state0 = 1;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v3: i32 = n;
// LOWERING-NEXT:                     let _v4: i32 = 0;
// LOWERING-NEXT:                     let _v5: bool = _v3 == _v4;
// LOWERING-NEXT:                     if _v5 {
// LOWERING-NEXT:                         __state0 = 2;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v6: i32 = 1;
// LOWERING-NEXT:                 __retval = _v6;
// LOWERING-NEXT:                 let _v7: i32 = __retval;
// LOWERING-NEXT:                 return _v7;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v8: i32 = -1;
// LOWERING-NEXT:                 __retval = _v8;
// LOWERING-NEXT:                 let _v9: i32 = __retval;
// LOWERING-NEXT:                 return _v9;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 let _v10: i32 = 0;
// LOWERING-NEXT:                 __retval = _v10;
// LOWERING-NEXT:                 let _v11: i32 = __retval;
// LOWERING-NEXT:                 return _v11;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = -5;
// LOWERING-NEXT:     let _v3: i32 = classify(_v2);
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     let _v5: i32 = classify(_v4);
// LOWERING-NEXT:     let _v6: i32 = 42;
// LOWERING-NEXT:     let _v7: i32 = classify(_v6);
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
// REWRITES-NEXT: fn classify(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut n: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         n = arg0;
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let _v1: i32 = 0;
// REWRITES-NEXT:                                         if n < _v1 {
// REWRITES-NEXT:                                                             __state0 = 1;
// REWRITES-NEXT:                                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let _v4: i32 = 0;
// REWRITES-NEXT:                                         if n == _v4 {
// REWRITES-NEXT:                                                             __state0 = 2;
// REWRITES-NEXT:                                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         __retval = 1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         __retval = -1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = -5;
// REWRITES-NEXT: let _v3: i32 = classify(_v2);
// REWRITES-NEXT: let _v4: i32 = 0;
// REWRITES-NEXT: let _v5: i32 = classify(_v4);
// REWRITES-NEXT: let _v6: i32 = 42;
// REWRITES-NEXT: let _v7: i32 = classify(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
