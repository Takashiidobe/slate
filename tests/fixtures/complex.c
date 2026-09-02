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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 1.0, im: 2.0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 3.0, im: 4.0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { __muldc3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { __divdc3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 100.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 100.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
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
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 1.0, im: 2.0 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 3.0, im: 4.0 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}} - {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} != {{_v[0-9]+}} { {{_v[0-9]+}} != {{_v[0-9]+}} } else { false };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { __muldc3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = unsafe { __divdc3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}.re as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}.im as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}.re as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}.im as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}.re as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}.im as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = 100.0 * {{_v[0-9]+}}.re;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = 100.0 * {{_v[0-9]+}}.im;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
