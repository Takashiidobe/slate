#include <stdio.h>

static int first_multiple(int n, int m) {
  for (int i = 1; i <= n; i++) {
    if (i % m == 0) {
      return i;
    }
    if (i > 100) {
      break;
    }
  }
  return -1;
}

int main(void) {
  printf("%d\n", first_multiple(50, 7));
  printf("%d\n", first_multiple(3, 7));
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
// LOWERING-NEXT: fn first_multiple(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut m: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     n = arg0;
// LOWERING-NEXT:     m = arg1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v0: i32 = 1;
// LOWERING-NEXT:         i = _v0;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = i;
// LOWERING-NEXT:             let _v2: i32 = n;
// LOWERING-NEXT:             let _v3: bool = _v1 <= _v2;
// LOWERING-NEXT:             if !_v3 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v4: i32 = i;
// LOWERING-NEXT:                     let _v5: i32 = m;
// LOWERING-NEXT:                     let _v6: i32 = _v4 % _v5;
// LOWERING-NEXT:                     let _v7: i32 = 0;
// LOWERING-NEXT:                     let _v8: bool = _v6 == _v7;
// LOWERING-NEXT:                     if _v8 {
// LOWERING-NEXT:                         let _v9: i32 = i;
// LOWERING-NEXT:                         __retval = _v9;
// LOWERING-NEXT:                         let _v10: i32 = __retval;
// LOWERING-NEXT:                         return _v10;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v11: i32 = i;
// LOWERING-NEXT:                     let _v12: i32 = 100;
// LOWERING-NEXT:                     let _v13: bool = _v11 > _v12;
// LOWERING-NEXT:                     if _v13 {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v14: i32 = i;
// LOWERING-NEXT:             let _v15: i32 = _v14 + 1;
// LOWERING-NEXT:             i = _v15;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v16: i32 = -1;
// LOWERING-NEXT:     __retval = _v16;
// LOWERING-NEXT:     let _v17: i32 = __retval;
// LOWERING-NEXT:     return _v17;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 50;
// LOWERING-NEXT:     let _v3: i32 = 7;
// LOWERING-NEXT:     let _v4: i32 = first_multiple(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = 3;
// LOWERING-NEXT:     let _v8: i32 = 7;
// LOWERING-NEXT:     let _v9: i32 = first_multiple(_v7, _v8);
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
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
// REWRITES-NEXT: fn first_multiple(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut n: i32 = arg0;
// REWRITES-NEXT: let mut m: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 1;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i <= n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let _v7: i32 = 0;
// REWRITES-NEXT:                                                         if i % m == _v7 {
// REWRITES-NEXT:                                                                                 __retval = i;
// REWRITES-NEXT:                                                                                 return __retval;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let _v12: i32 = 100;
// REWRITES-NEXT:                                                         if i > _v12 {
// REWRITES-NEXT:                                                                                 break;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = -1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 50;
// REWRITES-NEXT: let _v3: i32 = 7;
// REWRITES-NEXT: let _v4: i32 = first_multiple(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = 3;
// REWRITES-NEXT: let _v8: i32 = 7;
// REWRITES-NEXT: let _v9: i32 = first_multiple(_v7, _v8);
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
