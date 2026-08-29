#include <stdio.h>

static int countdown(int n) {
  int steps = 0;
  while (1) {
    if (n <= 0) {
      break;
    }
    n /= 2;
    steps++;
  }
  return steps;
}

int main(void) {
  printf("%d\n", countdown(100));
  printf("%d\n", countdown(0));
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
// LOWERING-NEXT: fn countdown(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut steps: i32 = 0;
// LOWERING-NEXT:     n = arg0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     steps = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = 1;
// LOWERING-NEXT:             let _v2: bool = _v1 != 0;
// LOWERING-NEXT:             if !_v2 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v3: i32 = n;
// LOWERING-NEXT:                     let _v4: i32 = 0;
// LOWERING-NEXT:                     let _v5: bool = _v3 <= _v4;
// LOWERING-NEXT:                     if _v5 {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v6: i32 = 2;
// LOWERING-NEXT:                 let _v7: i32 = n;
// LOWERING-NEXT:                 let _v8: i32 = _v7 / _v6;
// LOWERING-NEXT:                 n = _v8;
// LOWERING-NEXT:                 let _v9: i32 = steps;
// LOWERING-NEXT:                 let _v10: i32 = _v9 + 1;
// LOWERING-NEXT:                 steps = _v10;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v11: i32 = steps;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     return _v12;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 100;
// LOWERING-NEXT:     let _v3: i32 = countdown(_v2);
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     let _v7: i32 = countdown(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
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
// REWRITES-NEXT: fn countdown(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut n: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut steps: i32 = 0;
// REWRITES-NEXT: steps = 0;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v1: i32 = 1;
// REWRITES-NEXT:         let _v2: bool = _v1 != 0;
// REWRITES-NEXT:         if !_v2 {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v4: i32 = 0;
// REWRITES-NEXT:                                     if n <= _v4 {
// REWRITES-NEXT:                                                         break;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v6: i32 = 2;
// REWRITES-NEXT:                     n = n / _v6;
// REWRITES-NEXT:                     steps = steps + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = steps;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 100;
// REWRITES-NEXT: let _v3: i32 = countdown(_v2);
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i32 = 0;
// REWRITES-NEXT: let _v7: i32 = countdown(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
