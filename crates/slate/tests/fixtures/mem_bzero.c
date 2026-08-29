#include <stdio.h>

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};
  __builtin_bzero(a, sizeof(a));
  int sum = 0;
  for (int i = 0; i < 5; i++)
    sum += a[i];
  printf("%d\n", sum);
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
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *a = [1, 2, 3, 4, 5];
// LOWERING-NEXT:     let _v1: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: u64 = 20;
// LOWERING-NEXT:     let _v4: u8 = 0;
// LOWERING-NEXT:     unsafe { std::ptr::write_bytes(_v2 as *mut u8, _v4 as u8, _v3 as usize) };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     sum = _v5;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v6: i32 = 0;
// LOWERING-NEXT:         i = _v6;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v7: i32 = i;
// LOWERING-NEXT:             let _v8: i32 = 5;
// LOWERING-NEXT:             let _v9: bool = _v7 < _v8;
// LOWERING-NEXT:             if !_v9 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v10: i32 = i;
// LOWERING-NEXT:             let _v11: i64 = _v10 as i64;
// LOWERING-NEXT:             let _v12: i32 = a[(_v11 as usize)];
// LOWERING-NEXT:             let _v13: i32 = sum;
// LOWERING-NEXT:             let _v14: i32 = _v13 + _v12;
// LOWERING-NEXT:             sum = _v14;
// LOWERING-NEXT:             let _v15: i32 = i;
// LOWERING-NEXT:             let _v16: i32 = _v15 + 1;
// LOWERING-NEXT:             i = _v16;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: i32 = sum;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v17 as *const i8, _v18) };
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
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT: let mut sum: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *a = [1, 2, 3, 4, 5];
// REWRITES-NEXT: let _v1: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v3: u64 = 20;
// REWRITES-NEXT: let _v4: u8 = 0;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes((_v1 as *mut core::ffi::c_void) as *mut u8, _v4 as u8, _v3 as usize) };
// REWRITES-NEXT: sum = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v8: i32 = 5;
// REWRITES-NEXT:                     if !(i < _v8) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     sum = sum + a[((i as i64) as usize)];
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v17 as *const i8, sum) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
