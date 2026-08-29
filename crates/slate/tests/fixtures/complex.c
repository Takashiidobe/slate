#include <stdio.h>

int main(void) {
  double _Complex a    = __builtin_complex(1.0, 2.0);
  double _Complex b    = __builtin_complex(3.0, 4.0);
  double _Complex sum  = a + b;
  double _Complex diff = a - b;
  double _Complex prod = a * b;
  double _Complex quot = a / b;
  printf("%d\n", (int)__real__ sum);
  printf("%d\n", (int)__imag__ sum);
  printf("%d\n", (int)__real__ diff);
  printf("%d\n", (int)__imag__ diff);
  printf("%d\n", (int)__real__ prod);
  printf("%d\n", (int)__imag__ prod);
  printf("%d\n", (int)(100.0 * __real__ quot));
  printf("%d\n", (int)(100.0 * __imag__ quot));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut b: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut sum: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut diff: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut prod: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut quot: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: num_complex::Complex<f64> = num_complex::Complex { re: 1.0, im: 2.0 };
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: num_complex::Complex<f64> = num_complex::Complex { re: 3.0, im: 4.0 };
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: num_complex::Complex<f64> = a;
// LOWERING-NEXT:     let _v4: num_complex::Complex<f64> = b;
// LOWERING-NEXT:     let _v5: num_complex::Complex<f64> = num_complex::Complex { re: _v3.re + _v4.re, im: _v3.im + _v4.im };
// LOWERING-NEXT:     sum = _v5;
// LOWERING-NEXT:     let _v6: num_complex::Complex<f64> = a;
// LOWERING-NEXT:     let _v7: num_complex::Complex<f64> = b;
// LOWERING-NEXT:     let _v8: num_complex::Complex<f64> = num_complex::Complex { re: _v6.re - _v7.re, im: _v6.im - _v7.im };
// LOWERING-NEXT:     diff = _v8;
// LOWERING-NEXT:     let _v9: num_complex::Complex<f64> = a;
// LOWERING-NEXT:     let _v10: num_complex::Complex<f64> = b;
// LOWERING-NEXT:     let _v11: f64 = _v9.re;
// LOWERING-NEXT:     let _v12: f64 = _v9.im;
// LOWERING-NEXT:     let _v13: f64 = _v10.re;
// LOWERING-NEXT:     let _v14: f64 = _v10.im;
// LOWERING-NEXT:     let _v15: f64 = _v11 * _v13;
// LOWERING-NEXT:     let _v16: f64 = _v12 * _v14;
// LOWERING-NEXT:     let _v17: f64 = _v11 * _v14;
// LOWERING-NEXT:     let _v18: f64 = _v12 * _v13;
// LOWERING-NEXT:     let _v19: f64 = _v15 - _v16;
// LOWERING-NEXT:     let _v20: f64 = _v17 + _v18;
// LOWERING-NEXT:     let _v21: num_complex::Complex<f64> = num_complex::Complex { re: _v19, im: _v20 };
// LOWERING-NEXT:     let _v22: bool = _v19 != _v19;
// LOWERING-NEXT:     let _v23: bool = _v20 != _v20;
// LOWERING-NEXT:     let _v24: bool = false;
// LOWERING-NEXT:     let _v25: bool = if _v22 { _v23 } else { _v24 };
// LOWERING-NEXT:     let _v26: num_complex::Complex<f64> = if _v25 {
// LOWERING-NEXT:         let _v27: num_complex::Complex<f64> = unsafe { __muldc3(_v11, _v12, _v13, _v14) };
// LOWERING-NEXT:         _v27
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         _v21
// LOWERING-NEXT:     };
// LOWERING-NEXT:     prod = _v26;
// LOWERING-NEXT:     let _v28: num_complex::Complex<f64> = a;
// LOWERING-NEXT:     let _v29: num_complex::Complex<f64> = b;
// LOWERING-NEXT:     let _v30: f64 = _v28.re;
// LOWERING-NEXT:     let _v31: f64 = _v28.im;
// LOWERING-NEXT:     let _v32: f64 = _v29.re;
// LOWERING-NEXT:     let _v33: f64 = _v29.im;
// LOWERING-NEXT:     let _v34: num_complex::Complex<f64> = unsafe { __divdc3(_v30, _v31, _v32, _v33) };
// LOWERING-NEXT:     quot = _v34;
// LOWERING-NEXT:     let _v35: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v36: num_complex::Complex<f64> = sum;
// LOWERING-NEXT:     let _v37: f64 = _v36.re;
// LOWERING-NEXT:     let _v38: i32 = _v37 as i32;
// LOWERING-NEXT:     let _v39: i32 = unsafe { printf(_v35 as *const i8, _v38) };
// LOWERING-NEXT:     let _v40: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v41: num_complex::Complex<f64> = sum;
// LOWERING-NEXT:     let _v42: f64 = _v41.im;
// LOWERING-NEXT:     let _v43: i32 = _v42 as i32;
// LOWERING-NEXT:     let _v44: i32 = unsafe { printf(_v40 as *const i8, _v43) };
// LOWERING-NEXT:     let _v45: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v46: num_complex::Complex<f64> = diff;
// LOWERING-NEXT:     let _v47: f64 = _v46.re;
// LOWERING-NEXT:     let _v48: i32 = _v47 as i32;
// LOWERING-NEXT:     let _v49: i32 = unsafe { printf(_v45 as *const i8, _v48) };
// LOWERING-NEXT:     let _v50: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v51: num_complex::Complex<f64> = diff;
// LOWERING-NEXT:     let _v52: f64 = _v51.im;
// LOWERING-NEXT:     let _v53: i32 = _v52 as i32;
// LOWERING-NEXT:     let _v54: i32 = unsafe { printf(_v50 as *const i8, _v53) };
// LOWERING-NEXT:     let _v55: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v56: num_complex::Complex<f64> = prod;
// LOWERING-NEXT:     let _v57: f64 = _v56.re;
// LOWERING-NEXT:     let _v58: i32 = _v57 as i32;
// LOWERING-NEXT:     let _v59: i32 = unsafe { printf(_v55 as *const i8, _v58) };
// LOWERING-NEXT:     let _v60: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v61: num_complex::Complex<f64> = prod;
// LOWERING-NEXT:     let _v62: f64 = _v61.im;
// LOWERING-NEXT:     let _v63: i32 = _v62 as i32;
// LOWERING-NEXT:     let _v64: i32 = unsafe { printf(_v60 as *const i8, _v63) };
// LOWERING-NEXT:     let _v65: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v66: f64 = 100.0;
// LOWERING-NEXT:     let _v67: num_complex::Complex<f64> = quot;
// LOWERING-NEXT:     let _v68: f64 = _v67.re;
// LOWERING-NEXT:     let _v69: f64 = _v66 * _v68;
// LOWERING-NEXT:     let _v70: i32 = _v69 as i32;
// LOWERING-NEXT:     let _v71: i32 = unsafe { printf(_v65 as *const i8, _v70) };
// LOWERING-NEXT:     let _v72: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v73: f64 = 100.0;
// LOWERING-NEXT:     let _v74: num_complex::Complex<f64> = quot;
// LOWERING-NEXT:     let _v75: f64 = _v74.im;
// LOWERING-NEXT:     let _v76: f64 = _v73 * _v75;
// LOWERING-NEXT:     let _v77: i32 = _v76 as i32;
// LOWERING-NEXT:     let _v78: i32 = unsafe { printf(_v72 as *const i8, _v77) };
// LOWERING-NEXT:     let _v79: i32 = 0;
// LOWERING-NEXT:     __retval = _v79;
// LOWERING-NEXT:     let _v80: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v80 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut b: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut sum: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut diff: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut prod: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut quot: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = num_complex::Complex { re: 1.0, im: 2.0 };
// REWRITES-NEXT: b = num_complex::Complex { re: 3.0, im: 4.0 };
// REWRITES-NEXT: let _v3: num_complex::Complex<f64> = a;
// REWRITES-NEXT: let _v4: num_complex::Complex<f64> = b;
// REWRITES-NEXT: sum = num_complex::Complex { re: _v3.re + _v4.re, im: _v3.im + _v4.im };
// REWRITES-NEXT: let _v6: num_complex::Complex<f64> = a;
// REWRITES-NEXT: let _v7: num_complex::Complex<f64> = b;
// REWRITES-NEXT: diff = num_complex::Complex { re: _v6.re - _v7.re, im: _v6.im - _v7.im };
// REWRITES-NEXT: let _v9: num_complex::Complex<f64> = a;
// REWRITES-NEXT: let _v10: num_complex::Complex<f64> = b;
// REWRITES-NEXT: let _v11: f64 = _v9.re;
// REWRITES-NEXT: let _v12: f64 = _v9.im;
// REWRITES-NEXT: let _v13: f64 = _v10.re;
// REWRITES-NEXT: let _v14: f64 = _v10.im;
// REWRITES-NEXT: let _v19: f64 = _v11 * _v13 - _v12 * _v14;
// REWRITES-NEXT: let _v20: f64 = _v11 * _v14 + _v12 * _v13;
// REWRITES-NEXT: let _v21: num_complex::Complex<f64> = num_complex::Complex { re: _v19, im: _v20 };
// REWRITES-NEXT: let _v25: bool = if _v19 != _v19 { _v20 != _v20 } else { false };
// REWRITES-NEXT: let _v26: num_complex::Complex<f64> = if _v25 {
// REWRITES-NEXT:         let _v27: num_complex::Complex<f64> = unsafe { __muldc3(_v11, _v12, _v13, _v14) };
// REWRITES-NEXT:     _v27
// REWRITES-NEXT: } else {
// REWRITES-NEXT:     _v21
// REWRITES-NEXT: };
// REWRITES-NEXT: prod = _v26;
// REWRITES-NEXT: let _v28: num_complex::Complex<f64> = a;
// REWRITES-NEXT: let _v29: num_complex::Complex<f64> = b;
// REWRITES-NEXT: let _v30: f64 = _v28.re;
// REWRITES-NEXT: let _v31: f64 = _v28.im;
// REWRITES-NEXT: let _v32: f64 = _v29.re;
// REWRITES-NEXT: let _v33: f64 = _v29.im;
// REWRITES-NEXT: quot = unsafe { __divdc3(_v30, _v31, _v32, _v33) };
// REWRITES-NEXT: let _v35: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v36: num_complex::Complex<f64> = sum;
// REWRITES-NEXT: let _v39: i32 = unsafe { printf(_v35 as *const i8, _v36.re as i32) };
// REWRITES-NEXT: let _v40: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v41: num_complex::Complex<f64> = sum;
// REWRITES-NEXT: let _v44: i32 = unsafe { printf(_v40 as *const i8, _v41.im as i32) };
// REWRITES-NEXT: let _v45: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v46: num_complex::Complex<f64> = diff;
// REWRITES-NEXT: let _v49: i32 = unsafe { printf(_v45 as *const i8, _v46.re as i32) };
// REWRITES-NEXT: let _v50: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v51: num_complex::Complex<f64> = diff;
// REWRITES-NEXT: let _v54: i32 = unsafe { printf(_v50 as *const i8, _v51.im as i32) };
// REWRITES-NEXT: let _v55: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v56: num_complex::Complex<f64> = prod;
// REWRITES-NEXT: let _v59: i32 = unsafe { printf(_v55 as *const i8, _v56.re as i32) };
// REWRITES-NEXT: let _v60: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v61: num_complex::Complex<f64> = prod;
// REWRITES-NEXT: let _v64: i32 = unsafe { printf(_v60 as *const i8, _v61.im as i32) };
// REWRITES-NEXT: let _v65: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v66: f64 = 100.0;
// REWRITES-NEXT: let _v67: num_complex::Complex<f64> = quot;
// REWRITES-NEXT: let _v71: i32 = unsafe { printf(_v65 as *const i8, (_v66 * _v67.re) as i32) };
// REWRITES-NEXT: let _v72: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v73: f64 = 100.0;
// REWRITES-NEXT: let _v74: num_complex::Complex<f64> = quot;
// REWRITES-NEXT: let _v78: i32 = unsafe { printf(_v72 as *const i8, (_v73 * _v74.im) as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
