#include <complex.h>
#include <stdio.h>

int main(void) {
  volatile double real      = 0.5;
  volatile double imaginary = -0.25;
  double complex  a         = __builtin_complex(real, imaginary);
  double complex  b         = __builtin_complex(real, -imaginary);
  double complex  product;
  double complex  quotient;

  {
#pragma STDC CX_LIMITED_RANGE ON
    product = a * b;
  }

  {
#pragma STDC CX_LIMITED_RANGE OFF
    quotient = a / b;
  }

  printf("%.4f %.4f %.4f %.4f\n", creal(product), cimag(product),
         creal(quotient), cimag(quotient));
  return creal(product) == 0.3125 && cimag(product) == 0.0 &&
                 creal(quotient) == 0.6 && cimag(quotient) == -0.8
             ? 0
             : 1;
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
// LOWERING-NEXT:     let mut real: f64 = 0.0;
// LOWERING-NEXT:     let mut imaginary: f64 = 0.0;
// LOWERING-NEXT:     let mut a: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut b: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut product: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut quotient: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 0.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), _v1) };
// LOWERING-NEXT:     let _v2: f64 = -0.25;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(imaginary), _v2) };
// LOWERING-NEXT:     let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:     let _v4: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// LOWERING-NEXT:     let _v5: num_complex::Complex<f64> = num_complex::Complex { re: _v3, im: _v4 };
// LOWERING-NEXT:     a = _v5;
// LOWERING-NEXT:     let _v6: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:     let _v7: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// LOWERING-NEXT:     let _v8: f64 = -_v7;
// LOWERING-NEXT:     let _v9: num_complex::Complex<f64> = num_complex::Complex { re: _v6, im: _v8 };
// LOWERING-NEXT:     b = _v9;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v10: num_complex::Complex<f64> = a;
// LOWERING-NEXT:         let _v11: num_complex::Complex<f64> = b;
// LOWERING-NEXT:         let _v12: f64 = _v10.re;
// LOWERING-NEXT:         let _v13: f64 = _v10.im;
// LOWERING-NEXT:         let _v14: f64 = _v11.re;
// LOWERING-NEXT:         let _v15: f64 = _v11.im;
// LOWERING-NEXT:         let _v16: f64 = _v12 * _v14;
// LOWERING-NEXT:         let _v17: f64 = _v13 * _v15;
// LOWERING-NEXT:         let _v18: f64 = _v12 * _v15;
// LOWERING-NEXT:         let _v19: f64 = _v13 * _v14;
// LOWERING-NEXT:         let _v20: f64 = _v16 - _v17;
// LOWERING-NEXT:         let _v21: f64 = _v18 + _v19;
// LOWERING-NEXT:         let _v22: num_complex::Complex<f64> = num_complex::Complex { re: _v20, im: _v21 };
// LOWERING-NEXT:         product = _v22;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v23: num_complex::Complex<f64> = a;
// LOWERING-NEXT:         let _v24: num_complex::Complex<f64> = b;
// LOWERING-NEXT:         let _v25: f64 = _v23.re;
// LOWERING-NEXT:         let _v26: f64 = _v23.im;
// LOWERING-NEXT:         let _v27: f64 = _v24.re;
// LOWERING-NEXT:         let _v28: f64 = _v24.im;
// LOWERING-NEXT:         let _v29: num_complex::Complex<f64> = unsafe { __divdc3(_v25, _v26, _v27, _v28) };
// LOWERING-NEXT:         quotient = _v29;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v30: *mut i8 = b"%.4f %.4f %.4f %.4f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v31: num_complex::Complex<f64> = product;
// LOWERING-NEXT:     let _v32: f64 = _v31.re;
// LOWERING-NEXT:     let _v33: num_complex::Complex<f64> = product;
// LOWERING-NEXT:     let _v34: f64 = _v33.im;
// LOWERING-NEXT:     let _v35: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:     let _v36: f64 = _v35.re;
// LOWERING-NEXT:     let _v37: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:     let _v38: f64 = _v37.im;
// LOWERING-NEXT:     let _v39: i32 = unsafe { printf(_v30 as *const i8, _v32, _v34, _v36, _v38) };
// LOWERING-NEXT:     let _v40: num_complex::Complex<f64> = product;
// LOWERING-NEXT:     let _v41: f64 = _v40.re;
// LOWERING-NEXT:     let _v42: f64 = 0.3125;
// LOWERING-NEXT:     let _v43: bool = _v41 == _v42;
// LOWERING-NEXT:     let _v44: bool = if _v43 {
// LOWERING-NEXT:         let _v45: num_complex::Complex<f64> = product;
// LOWERING-NEXT:         let _v46: f64 = _v45.im;
// LOWERING-NEXT:         let _v47: f64 = 0.0;
// LOWERING-NEXT:         let _v48: bool = _v46 == _v47;
// LOWERING-NEXT:         _v48
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v49: bool = false;
// LOWERING-NEXT:         _v49
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v50: bool = if _v44 {
// LOWERING-NEXT:         let _v51: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:         let _v52: f64 = _v51.re;
// LOWERING-NEXT:         let _v53: f64 = 0.6;
// LOWERING-NEXT:         let _v54: bool = _v52 == _v53;
// LOWERING-NEXT:         _v54
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v55: bool = false;
// LOWERING-NEXT:         _v55
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v56: bool = if _v50 {
// LOWERING-NEXT:         let _v57: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:         let _v58: f64 = _v57.im;
// LOWERING-NEXT:         let _v59: f64 = -0.8;
// LOWERING-NEXT:         let _v60: bool = _v58 == _v59;
// LOWERING-NEXT:         _v60
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v61: bool = false;
// LOWERING-NEXT:         _v61
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v62: i32 = 0;
// LOWERING-NEXT:     let _v63: i32 = 1;
// LOWERING-NEXT:     let _v64: i32 = if _v56 { _v62 } else { _v63 };
// LOWERING-NEXT:     __retval = _v64;
// LOWERING-NEXT:     let _v65: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v65 as i32);
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
// REWRITES-NEXT: let mut real: f64 = 0.0;
// REWRITES-NEXT: let mut imaginary: f64 = 0.0;
// REWRITES-NEXT: let mut a: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut b: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut product: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut quotient: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: f64 = 0.5;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), _v1) };
// REWRITES-NEXT: let _v2: f64 = -0.25;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(imaginary), _v2) };
// REWRITES-NEXT: let _v3: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// REWRITES-NEXT: let _v4: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// REWRITES-NEXT: a = num_complex::Complex { re: _v3, im: _v4 };
// REWRITES-NEXT: let _v6: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// REWRITES-NEXT: let _v7: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// REWRITES-NEXT: b = num_complex::Complex { re: _v6, im: -_v7 };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v10: num_complex::Complex<f64> = a;
// REWRITES-NEXT:         let _v11: num_complex::Complex<f64> = b;
// REWRITES-NEXT:         let _v12: f64 = _v10.re;
// REWRITES-NEXT:         let _v13: f64 = _v10.im;
// REWRITES-NEXT:         let _v14: f64 = _v11.re;
// REWRITES-NEXT:         let _v15: f64 = _v11.im;
// REWRITES-NEXT:         product = num_complex::Complex { re: _v12 * _v14 - _v13 * _v15, im: _v12 * _v15 + _v13 * _v14 };
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v23: num_complex::Complex<f64> = a;
// REWRITES-NEXT:         let _v24: num_complex::Complex<f64> = b;
// REWRITES-NEXT:         let _v25: f64 = _v23.re;
// REWRITES-NEXT:         let _v26: f64 = _v23.im;
// REWRITES-NEXT:         let _v27: f64 = _v24.re;
// REWRITES-NEXT:         let _v28: f64 = _v24.im;
// REWRITES-NEXT:         quotient = unsafe { __divdc3(_v25, _v26, _v27, _v28) };
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v30: *mut i8 = b"%.4f %.4f %.4f %.4f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v31: num_complex::Complex<f64> = product;
// REWRITES-NEXT: let _v32: f64 = _v31.re;
// REWRITES-NEXT: let _v33: num_complex::Complex<f64> = product;
// REWRITES-NEXT: let _v34: f64 = _v33.im;
// REWRITES-NEXT: let _v35: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT: let _v36: f64 = _v35.re;
// REWRITES-NEXT: let _v37: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT: let _v38: f64 = _v37.im;
// REWRITES-NEXT: let _v39: i32 = unsafe { printf(_v30 as *const i8, _v32, _v34, _v36, _v38) };
// REWRITES-NEXT: let _v40: num_complex::Complex<f64> = product;
// REWRITES-NEXT: let _v42: f64 = 0.3125;
// REWRITES-NEXT: let _v44: bool = if _v40.re == _v42 {
// REWRITES-NEXT:         let _v45: num_complex::Complex<f64> = product;
// REWRITES-NEXT:         let _v47: f64 = 0.0;
// REWRITES-NEXT:         let _v48: bool = _v45.im == _v47;
// REWRITES-NEXT:     _v48
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v49: bool = false;
// REWRITES-NEXT:     _v49
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v50: bool = if _v44 {
// REWRITES-NEXT:         let _v51: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT:         let _v53: f64 = 0.6;
// REWRITES-NEXT:         let _v54: bool = _v51.re == _v53;
// REWRITES-NEXT:     _v54
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v55: bool = false;
// REWRITES-NEXT:     _v55
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v56: bool = if _v50 {
// REWRITES-NEXT:         let _v57: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT:         let _v59: f64 = -0.8;
// REWRITES-NEXT:         let _v60: bool = _v57.im == _v59;
// REWRITES-NEXT:     _v60
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v61: bool = false;
// REWRITES-NEXT:     _v61
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v62: i32 = 0;
// REWRITES-NEXT: let _v63: i32 = 1;
// REWRITES-NEXT: __retval = if _v56 { _v62 } else { _v63 };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
