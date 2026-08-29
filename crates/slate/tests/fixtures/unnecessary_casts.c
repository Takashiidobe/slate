#include <stdio.h>

int main(void) {
  unsigned char a = 200;
  unsigned char b = 100;
  unsigned char c = 0;
  for (int i = 0; i < 1; i++) {
    c = a + b;
  }
  printf("%d\n", c);
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
// LOWERING-NEXT:     let mut a: u8 = 0;
// LOWERING-NEXT:     let mut b: u8 = 0;
// LOWERING-NEXT:     let mut c: u8 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u8 = 200;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: u8 = 100;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: u8 = 0;
// LOWERING-NEXT:     c = _v3;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v4: i32 = 0;
// LOWERING-NEXT:         i = _v4;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v5: i32 = i;
// LOWERING-NEXT:             let _v6: i32 = 1;
// LOWERING-NEXT:             let _v7: bool = _v5 < _v6;
// LOWERING-NEXT:             if !_v7 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v8: u8 = a;
// LOWERING-NEXT:                 let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:                 let _v10: u8 = b;
// LOWERING-NEXT:                 let _v11: i32 = _v10 as i32;
// LOWERING-NEXT:                 let _v12: i32 = _v9 + _v11;
// LOWERING-NEXT:                 let _v13: u8 = _v12 as u8;
// LOWERING-NEXT:                 c = _v13;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v14: i32 = i;
// LOWERING-NEXT:             let _v15: i32 = _v14 + 1;
// LOWERING-NEXT:             i = _v15;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: u8 = c;
// LOWERING-NEXT:     let _v18: i32 = _v17 as i32;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v16 as *const i8, _v18) };
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
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: u8 = 0;
// REWRITES-NEXT: let mut b: u8 = 0;
// REWRITES-NEXT: let mut c: u8 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 200;
// REWRITES-NEXT: b = 100;
// REWRITES-NEXT: c = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v6: i32 = 1;
// REWRITES-NEXT:                     if !(i < _v6) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     c = ((a as i32) + (b as i32)) as u8;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v16 as *const i8, c as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
