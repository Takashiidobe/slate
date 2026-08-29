#include <stdio.h>

int main() {
  volatile int choose_b = 0;
  int          x        = 0;
  if (choose_b)
    goto b;
a:
  x = x + 1;
  if (x < 3)
    goto b;
  goto done;
b:
  x = x + 2;
  if (x < 4)
    goto a;
done:
  printf("%d\n", x);
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
// LOWERING-NEXT:     let mut choose_b: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 let _v0: i32 = 0;
// LOWERING-NEXT:                 __retval = _v0;
// LOWERING-NEXT:                 let _v1: i32 = 0;
// LOWERING-NEXT:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(choose_b), _v1) };
// LOWERING-NEXT:                 let _v2: i32 = 0;
// LOWERING-NEXT:                 x = _v2;
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(choose_b)) };
// LOWERING-NEXT:                     let _v4: bool = _v3 != 0;
// LOWERING-NEXT:                     if _v4 {
// LOWERING-NEXT:                         __state0 = 2;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 __state0 = 1;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v5: i32 = x;
// LOWERING-NEXT:                 let _v6: i32 = 1;
// LOWERING-NEXT:                 let _v7: i32 = _v5 + _v6;
// LOWERING-NEXT:                 x = _v7;
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v8: i32 = x;
// LOWERING-NEXT:                     let _v9: i32 = 3;
// LOWERING-NEXT:                     let _v10: bool = _v8 < _v9;
// LOWERING-NEXT:                     if _v10 {
// LOWERING-NEXT:                         __state0 = 2;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 __state0 = 3;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 let _v11: i32 = x;
// LOWERING-NEXT:                 let _v12: i32 = 2;
// LOWERING-NEXT:                 let _v13: i32 = _v11 + _v12;
// LOWERING-NEXT:                 x = _v13;
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v14: i32 = x;
// LOWERING-NEXT:                     let _v15: i32 = 4;
// LOWERING-NEXT:                     let _v16: bool = _v14 < _v15;
// LOWERING-NEXT:                     if _v16 {
// LOWERING-NEXT:                         __state0 = 1;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 __state0 = 3;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v18: i32 = x;
// LOWERING-NEXT:                 let _v19: i32 = unsafe { printf(_v17 as *const i8, _v18) };
// LOWERING-NEXT:                 let _v20: i32 = 0;
// LOWERING-NEXT:                 __retval = _v20;
// LOWERING-NEXT:                 let _v21: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit(_v21 as i32);
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 break '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
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
// REWRITES-NEXT: let mut choose_b: i32 = 0;
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         let _v1: i32 = 0;
// REWRITES-NEXT:                         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(choose_b), _v1) };
// REWRITES-NEXT:                         x = 0;
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(choose_b)) };
// REWRITES-NEXT:                                         let _v4: bool = _v3 != 0;
// REWRITES-NEXT:                                         if _v4 {
// REWRITES-NEXT:                                                             __state0 = 2;
// REWRITES-NEXT:                                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         __state0 = 1;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         let _v6: i32 = 1;
// REWRITES-NEXT:                         x = x + _v6;
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let _v9: i32 = 3;
// REWRITES-NEXT:                                         if x < _v9 {
// REWRITES-NEXT:                                                             __state0 = 2;
// REWRITES-NEXT:                                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         __state0 = 3;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                         let _v12: i32 = 2;
// REWRITES-NEXT:                         x = x + _v12;
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let _v15: i32 = 4;
// REWRITES-NEXT:                                         if x < _v15 {
// REWRITES-NEXT:                                                             __state0 = 1;
// REWRITES-NEXT:                                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         __state0 = 3;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                         let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                         let _v19: i32 = unsafe { printf(_v17 as *const i8, x) };
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         std::process::exit(__retval as i32);
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         break '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
