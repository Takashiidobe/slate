#include <stdio.h>

static __complex__ int global_value = 17 + 19i;

int main(void) {
  __complex__ int first     = 5 + 7i;
  __complex__ int second    = -3 + 11i;
  __complex__ int imaginary = 13i;
  printf("%d %d %d %d %d %d %d %d\n", __real__ first, __imag__ first,
         __real__ second, __imag__ second, __real__ imaginary,
         __imag__ imaginary, __real__ global_value, __imag__ global_value);
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
// LOWERING-NEXT: static mut global_value: num_complex::Complex<i32> = num_complex::Complex { re: 17, im: 19 };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut first: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 0 };
// LOWERING-NEXT:     let mut second: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 0 };
// LOWERING-NEXT:     let mut imaginary: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     let _v3: num_complex::Complex<i32> = num_complex::Complex { re: _v1, im: _v2 };
// LOWERING-NEXT:     let _v4: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 7 };
// LOWERING-NEXT:     let _v5: num_complex::Complex<i32> = num_complex::Complex { re: _v3.re + _v4.re, im: _v3.im + _v4.im };
// LOWERING-NEXT:     first = _v5;
// LOWERING-NEXT:     let _v6: i32 = -3;
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     let _v8: num_complex::Complex<i32> = num_complex::Complex { re: _v6, im: _v7 };
// LOWERING-NEXT:     let _v9: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 11 };
// LOWERING-NEXT:     let _v10: num_complex::Complex<i32> = num_complex::Complex { re: _v8.re + _v9.re, im: _v8.im + _v9.im };
// LOWERING-NEXT:     second = _v10;
// LOWERING-NEXT:     let _v11: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 13 };
// LOWERING-NEXT:     imaginary = _v11;
// LOWERING-NEXT:     let _v12: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: num_complex::Complex<i32> = first;
// LOWERING-NEXT:     let _v14: i32 = _v13.re;
// LOWERING-NEXT:     let _v15: num_complex::Complex<i32> = first;
// LOWERING-NEXT:     let _v16: i32 = _v15.im;
// LOWERING-NEXT:     let _v17: num_complex::Complex<i32> = second;
// LOWERING-NEXT:     let _v18: i32 = _v17.re;
// LOWERING-NEXT:     let _v19: num_complex::Complex<i32> = second;
// LOWERING-NEXT:     let _v20: i32 = _v19.im;
// LOWERING-NEXT:     let _v21: num_complex::Complex<i32> = imaginary;
// LOWERING-NEXT:     let _v22: i32 = _v21.re;
// LOWERING-NEXT:     let _v23: num_complex::Complex<i32> = imaginary;
// LOWERING-NEXT:     let _v24: i32 = _v23.im;
// LOWERING-NEXT:     let _v25: num_complex::Complex<i32> = unsafe { global_value };
// LOWERING-NEXT:     let _v26: i32 = _v25.re;
// LOWERING-NEXT:     let _v27: num_complex::Complex<i32> = unsafe { global_value };
// LOWERING-NEXT:     let _v28: i32 = _v27.im;
// LOWERING-NEXT:     let _v29: i32 = unsafe { printf(_v12 as *const i8, _v14, _v16, _v18, _v20, _v22, _v24, _v26, _v28) };
// LOWERING-NEXT:     let _v30: i32 = 0;
// LOWERING-NEXT:     __retval = _v30;
// LOWERING-NEXT:     let _v31: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v31 as i32);
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
// REWRITES-NEXT: static mut global_value: num_complex::Complex<i32> = num_complex::Complex { re: 17, im: 19 };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut first: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 0 };
// REWRITES-NEXT: let mut second: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 0 };
// REWRITES-NEXT: let mut imaginary: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 5;
// REWRITES-NEXT: let _v2: i32 = 0;
// REWRITES-NEXT: let _v3: num_complex::Complex<i32> = num_complex::Complex { re: _v1, im: _v2 };
// REWRITES-NEXT: let _v4: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 7 };
// REWRITES-NEXT: first = num_complex::Complex { re: _v3.re + _v4.re, im: _v3.im + _v4.im };
// REWRITES-NEXT: let _v6: i32 = -3;
// REWRITES-NEXT: let _v7: i32 = 0;
// REWRITES-NEXT: let _v8: num_complex::Complex<i32> = num_complex::Complex { re: _v6, im: _v7 };
// REWRITES-NEXT: let _v9: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 11 };
// REWRITES-NEXT: second = num_complex::Complex { re: _v8.re + _v9.re, im: _v8.im + _v9.im };
// REWRITES-NEXT: imaginary = num_complex::Complex { re: 0, im: 13 };
// REWRITES-NEXT: let _v12: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: num_complex::Complex<i32> = first;
// REWRITES-NEXT: let _v14: i32 = _v13.re;
// REWRITES-NEXT: let _v15: num_complex::Complex<i32> = first;
// REWRITES-NEXT: let _v16: i32 = _v15.im;
// REWRITES-NEXT: let _v17: num_complex::Complex<i32> = second;
// REWRITES-NEXT: let _v18: i32 = _v17.re;
// REWRITES-NEXT: let _v19: num_complex::Complex<i32> = second;
// REWRITES-NEXT: let _v20: i32 = _v19.im;
// REWRITES-NEXT: let _v21: num_complex::Complex<i32> = imaginary;
// REWRITES-NEXT: let _v22: i32 = _v21.re;
// REWRITES-NEXT: let _v23: num_complex::Complex<i32> = imaginary;
// REWRITES-NEXT: let _v24: i32 = _v23.im;
// REWRITES-NEXT: let _v25: num_complex::Complex<i32> = unsafe { global_value };
// REWRITES-NEXT: let _v26: i32 = _v25.re;
// REWRITES-NEXT: let _v27: num_complex::Complex<i32> = unsafe { global_value };
// REWRITES-NEXT: let _v28: i32 = _v27.im;
// REWRITES-NEXT: let _v29: i32 = unsafe { printf(_v12 as *const i8, _v14, _v16, _v18, _v20, _v22, _v24, _v26, _v28) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
