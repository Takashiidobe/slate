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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut real: f64 = 0.0;
// LOWERING-NEXT:     let mut imaginary: f64 = 0.0;
// LOWERING-NEXT:     let mut a: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut b: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut product: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut quotient: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = -0.25;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(imaginary), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = a;
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = b;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:         product = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = a;
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = b;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { __divdc3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:         quotient = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.4f %.4f %.4f %.4f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.3125;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.6;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = -0.8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut real: f64 = 0.0;
// REWRITES-NEXT:     let mut imaginary: f64 = 0.0;
// REWRITES-NEXT:     let mut a: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT:     let mut b: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT:     let mut product: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT:     let mut quotient: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), 0.5 as f64) };
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(imaginary), -0.25 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// REWRITES-NEXT:     a = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) };
// REWRITES-NEXT:     b = num_complex::Complex { re: {{_v[0-9]+}}, im: -{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     product = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}} * {{_v[0-9]+}} - {{_v[0-9]+}} * {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     quotient = unsafe { __divdc3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     unsafe { printf(c"%.4f %.4f %.4f %.4f\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.re == 0.3125;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = product;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.im == 0.0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.re == 0.6;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = quotient;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.im == -0.8;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
