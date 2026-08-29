#include <stdio.h>

int main() {
  int x = 1;
  goto done;
  x = 99;
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
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 let _v0: i32 = 0;
// LOWERING-NEXT:                 __retval = _v0;
// LOWERING-NEXT:                 let _v1: i32 = 1;
// LOWERING-NEXT:                 x = _v1;
// LOWERING-NEXT:                 __state0 = 2;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v2: i32 = 99;
// LOWERING-NEXT:                 x = _v2;
// LOWERING-NEXT:                 __state0 = 2;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v4: i32 = x;
// LOWERING-NEXT:                 let _v5: i32 = unsafe { printf(_v3 as *const i8, _v4) };
// LOWERING-NEXT:                 let _v6: i32 = 0;
// LOWERING-NEXT:                 __retval = _v6;
// LOWERING-NEXT:                 let _v7: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit(_v7 as i32);
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
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         x = 1;
// REWRITES-NEXT:                         __state0 = 2;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         x = 99;
// REWRITES-NEXT:                         __state0 = 2;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                         let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                         let _v5: i32 = unsafe { printf(_v3 as *const i8, x) };
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
