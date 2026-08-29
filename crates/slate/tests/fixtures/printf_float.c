#include <stdio.h>

int main(void) {
  float  x = 1.5f;
  double y = 2.25;
  printf("%f %.2f %.0f\n", x + 0.5f, y * 2.0, 3.0);
  printf("tail %.3f", 1.25);
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
// LOWERING-NEXT:     let mut x: f32 = 0.0;
// LOWERING-NEXT:     let mut y: f64 = 0.0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f32 = 1.5;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: f64 = 2.25;
// LOWERING-NEXT:     y = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%f %.2f %.0f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: f32 = x;
// LOWERING-NEXT:     let _v5: f32 = 0.5;
// LOWERING-NEXT:     let _v6: f32 = _v4 + _v5;
// LOWERING-NEXT:     let _v7: f64 = _v6 as f64;
// LOWERING-NEXT:     let _v8: f64 = y;
// LOWERING-NEXT:     let _v9: f64 = 2.0;
// LOWERING-NEXT:     let _v10: f64 = _v8 * _v9;
// LOWERING-NEXT:     let _v11: f64 = 3.0;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v3 as *const i8, _v7, _v10, _v11) };
// LOWERING-NEXT:     let _v13: *mut i8 = b"tail %.3f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: f64 = 1.25;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v13 as *const i8, _v14) };
// LOWERING-NEXT:     let _v16: i32 = 0;
// LOWERING-NEXT:     __retval = _v16;
// LOWERING-NEXT:     let _v17: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v17 as i32);
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
// REWRITES-NEXT: let mut x: f32 = 0.0;
// REWRITES-NEXT: let mut y: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 1.5;
// REWRITES-NEXT: y = 2.25;
// REWRITES-NEXT: let _v3: *mut i8 = b"%f %.2f %.0f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: f32 = 0.5;
// REWRITES-NEXT: let _v9: f64 = 2.0;
// REWRITES-NEXT: let _v11: f64 = 3.0;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v3 as *const i8, (x + _v5) as f64, y * _v9, _v11) };
// REWRITES-NEXT: let _v13: *mut i8 = b"tail %.3f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: f64 = 1.25;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v13 as *const i8, _v14) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
