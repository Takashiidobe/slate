#include <stdio.h>

static int sum_fixed(void) {
  int a[5]  = {1, 2, 3, 4, 5};
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += a[i];
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_fixed());
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
// LOWERING-NEXT: fn sum_fixed() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *a = [1, 2, 3, 4, 5];
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 5;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: i32 = a[(_v6 as usize)];
// LOWERING-NEXT:                 let _v8: i32 = total;
// LOWERING-NEXT:                 let _v9: i32 = _v8 + _v7;
// LOWERING-NEXT:                 total = _v9;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v10: i32 = i;
// LOWERING-NEXT:             let _v11: i32 = _v10 + 1;
// LOWERING-NEXT:             i = _v11;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: i32 = total;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     return _v13;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = sum_fixed();
// LOWERING-NEXT:     let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v5 as i32);
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
// REWRITES-NEXT: fn sum_fixed() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: *a = [1, 2, 3, 4, 5];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: i32 = 5;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     total = total + a[((i as i64) as usize)];
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = sum_fixed();
// REWRITES-NEXT: let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
