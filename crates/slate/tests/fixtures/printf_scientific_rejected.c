#include <stdio.h>

int main(void) {
  double x    = 1234.5678;
  int    prec = 2;
  printf("%015.2e\n", x);
  printf("%.*e\n", prec, x);
  printf("% e\n", x);
  printf("%#e\n", x);
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
// LOWERING-NEXT:     let mut x: f64 = 0.0;
// LOWERING-NEXT:     let mut prec: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 1234.5678;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: i32 = 2;
// LOWERING-NEXT:     prec = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%015.2e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: f64 = x;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v3 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%.*e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = prec;
// LOWERING-NEXT:     let _v8: f64 = x;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8) };
// LOWERING-NEXT:     let _v10: *mut i8 = b"% e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: f64 = x;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v10 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: *mut i8 = b"%#e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: f64 = x;
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
// REWRITES-NEXT: let mut x: f64 = 0.0;
// REWRITES-NEXT: let mut prec: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 1234.5678;
// REWRITES-NEXT: prec = 2;
// REWRITES-NEXT: let _v3: *mut i8 = b"%015.2e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v3 as *const i8, x) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%.*e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v6 as *const i8, prec, x) };
// REWRITES-NEXT: let _v10: *mut i8 = b"% e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v10 as *const i8, x) };
// REWRITES-NEXT: let _v13: *mut i8 = b"%#e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v13 as *const i8, x) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
