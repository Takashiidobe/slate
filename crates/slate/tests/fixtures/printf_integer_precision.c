#include <stdio.h>

int main(void) {
  int          a   = 5;
  int          neg = -5;
  unsigned int u   = 5u;
  unsigned int hex = 255u;
  printf("%.3d %.3d\n", a, neg);
  printf("%8.3d|%-8.3d|%+.3d\n", a, neg, a);
  printf("%08.3d\n", neg);
  printf("%.3u %.4x %.4X %.4o\n", u, hex, hex, hex);
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
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut neg: i32 = 0;
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let mut hex: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: i32 = -5;
// LOWERING-NEXT:     neg = _v2;
// LOWERING-NEXT:     let _v3: u32 = 5;
// LOWERING-NEXT:     u = _v3;
// LOWERING-NEXT:     let _v4: u32 = 255;
// LOWERING-NEXT:     hex = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%.3d %.3d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i32 = a;
// LOWERING-NEXT:     let _v7: i32 = neg;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v5 as *const i8, _v6, _v7) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%8.3d|%-8.3d|%+.3d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = a;
// LOWERING-NEXT:     let _v11: i32 = neg;
// LOWERING-NEXT:     let _v12: i32 = a;
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v9 as *const i8, _v10, _v11, _v12) };
// LOWERING-NEXT:     let _v14: *mut i8 = b"%08.3d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: i32 = neg;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v14 as *const i8, _v15) };
// LOWERING-NEXT:     let _v17: *mut i8 = b"%.3u %.4x %.4X %.4o\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: u32 = u;
// LOWERING-NEXT:     let _v19: u32 = hex;
// LOWERING-NEXT:     let _v20: u32 = hex;
// LOWERING-NEXT:     let _v21: u32 = hex;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v17 as *const i8, _v18, _v19, _v20, _v21) };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     __retval = _v23;
// LOWERING-NEXT:     let _v24: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v24 as i32);
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
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut neg: i32 = 0;
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: let mut hex: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 5;
// REWRITES-NEXT: neg = -5;
// REWRITES-NEXT: u = 5;
// REWRITES-NEXT: hex = 255;
// REWRITES-NEXT: let _v5: *mut i8 = b"%.3d %.3d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v5 as *const i8, a, neg) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%8.3d|%-8.3d|%+.3d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v9 as *const i8, a, neg, a) };
// REWRITES-NEXT: let _v14: *mut i8 = b"%08.3d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v14 as *const i8, neg) };
// REWRITES-NEXT: let _v17: *mut i8 = b"%.3u %.4x %.4X %.4o\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v17 as *const i8, u, hex, hex, hex) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
