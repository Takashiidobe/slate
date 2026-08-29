#include <stdio.h>

int main(void) {
  double a = 3.14159;
  double b = -3.14159;
  printf("%8.2f|%+8.2f|%08.2f|%-8.2f|%+.2f\n", a, a, b, a, b);
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
// LOWERING-NEXT:     let mut a: f64 = 0.0;
// LOWERING-NEXT:     let mut b: f64 = 0.0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 3.14159;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: f64 = -3.14159;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%8.2f|%+8.2f|%08.2f|%-8.2f|%+.2f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: f64 = a;
// LOWERING-NEXT:     let _v5: f64 = a;
// LOWERING-NEXT:     let _v6: f64 = b;
// LOWERING-NEXT:     let _v7: f64 = a;
// LOWERING-NEXT:     let _v8: f64 = b;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v3 as *const i8, _v4, _v5, _v6, _v7, _v8) };
// LOWERING-NEXT:     let _v10: i32 = 0;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v11 as i32);
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
// REWRITES-NEXT: let mut a: f64 = 0.0;
// REWRITES-NEXT: let mut b: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 3.14159;
// REWRITES-NEXT: b = -3.14159;
// REWRITES-NEXT: let _v3: *mut i8 = b"%8.2f|%+8.2f|%08.2f|%-8.2f|%+.2f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v3 as *const i8, a, a, b, a, b) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
