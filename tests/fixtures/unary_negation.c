#include <stdio.h>

static int neg_int(int x) { return -x; }

static double neg_double(double x) { return -x; }

int main(void) {
  printf("%d\n", neg_int(7));
  printf("%d\n", neg_int(-12));
  printf("%f\n", neg_double(1.5));
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
// LOWERING-NEXT: fn neg_int(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = _v0.wrapping_neg();
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn neg_double(arg0: f64) -> f64 {
// LOWERING-NEXT:     let mut x: f64 = 0.0;
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: f64 = x;
// LOWERING-NEXT:     let _v1: f64 = -_v0;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: f64 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 7;
// LOWERING-NEXT:     let _v3: i32 = neg_int(_v2);
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i32 = -12;
// LOWERING-NEXT:     let _v7: i32 = neg_int(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: f64 = 1.5;
// LOWERING-NEXT:     let _v11: f64 = neg_double(_v10);
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v9 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: i32 = 0;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v14 as i32);
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
// REWRITES-NEXT: fn neg_int(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = x;
// REWRITES-NEXT: __retval = _v0.wrapping_neg();
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn neg_double(arg0: f64) -> f64 {
// REWRITES-NEXT: let mut x: f64 = 0.0;
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: x = arg0;
// REWRITES-NEXT: __retval = -x;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 7;
// REWRITES-NEXT: let _v3: i32 = neg_int(_v2);
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i32 = -12;
// REWRITES-NEXT: let _v7: i32 = neg_int(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: f64 = 1.5;
// REWRITES-NEXT: let _v11: f64 = neg_double(_v10);
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v9 as *const i8, _v11) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
