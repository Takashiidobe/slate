#include <stdio.h>

static double pick(double *p) {
  *p = *p + 5.0;
  return *p;
}

int main(void) {
  double _Complex z = __builtin_complex(1.0, 2.0);
  __real__ z        = 7.0;
  __imag__ z        = 11.0;
  double r          = pick(&__real__ z);
  double i          = pick(&__imag__ z);
  printf("%d\n", (int)__real__ z);
  printf("%d\n", (int)__imag__ z);
  printf("%d\n", (int)r);
  printf("%d\n", (int)i);
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
// LOWERING-NEXT: fn pick(arg0: *mut f64) -> f64 {
// LOWERING-NEXT:     let mut p: *mut f64 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     let _v0: *mut f64 = p;
// LOWERING-NEXT:     let _v1: f64 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: f64 = 5.0;
// LOWERING-NEXT:     let _v3: f64 = _v1 + _v2;
// LOWERING-NEXT:     let _v4: *mut f64 = p;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v4 = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: *mut f64 = p;
// LOWERING-NEXT:     let _v6: f64 = unsafe { *_v5 };
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: f64 = __retval;
// LOWERING-NEXT:     return _v7;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut z: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut r: f64 = 0.0;
// LOWERING-NEXT:     let mut i: f64 = 0.0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: num_complex::Complex<f64> = num_complex::Complex { re: 1.0, im: 2.0 };
// LOWERING-NEXT:     z = _v1;
// LOWERING-NEXT:     let _v2: f64 = 7.0;
// LOWERING-NEXT:     z.re = _v2;
// LOWERING-NEXT:     let _v3: f64 = 11.0;
// LOWERING-NEXT:     z.im = _v3;
// LOWERING-NEXT:     let _v4: f64 = pick(std::ptr::addr_of_mut!(z.re));
// LOWERING-NEXT:     r = _v4;
// LOWERING-NEXT:     let _v5: f64 = pick(std::ptr::addr_of_mut!(z.im));
// LOWERING-NEXT:     i = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: num_complex::Complex<f64> = z;
// LOWERING-NEXT:     let _v8: f64 = _v7.re;
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: num_complex::Complex<f64> = z;
// LOWERING-NEXT:     let _v13: f64 = _v12.im;
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v11 as *const i8, _v14) };
// LOWERING-NEXT:     let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: f64 = r;
// LOWERING-NEXT:     let _v18: i32 = _v17 as i32;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v16 as *const i8, _v18) };
// LOWERING-NEXT:     let _v20: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: f64 = i;
// LOWERING-NEXT:     let _v22: i32 = _v21 as i32;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v20 as *const i8, _v22) };
// LOWERING-NEXT:     let _v24: i32 = 0;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v25 as i32);
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
// REWRITES-NEXT: fn pick(arg0: &mut f64) -> f64 {
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: let _v2: f64 = 5.0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *arg0 = (unsafe { *arg0 }) + _v2;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = unsafe { *arg0 };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut z: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut r: f64 = 0.0;
// REWRITES-NEXT: let mut i: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: z = num_complex::Complex { re: 1.0, im: 2.0 };
// REWRITES-NEXT: z.re = 7.0;
// REWRITES-NEXT: z.im = 11.0;
// REWRITES-NEXT: r = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.re)) });
// REWRITES-NEXT: i = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.im)) });
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: num_complex::Complex<f64> = z;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v7.re as i32) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: num_complex::Complex<f64> = z;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v11 as *const i8, _v12.im as i32) };
// REWRITES-NEXT: let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v16 as *const i8, r as i32) };
// REWRITES-NEXT: let _v20: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v20 as *const i8, i as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
