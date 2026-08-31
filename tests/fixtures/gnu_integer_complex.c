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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 7 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 11 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 13 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = unsafe { global_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<i32> = unsafe { global_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 7 };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 11 };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = num_complex::Complex { re: 0, im: 13 };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = unsafe { global_value };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<i32> = unsafe { global_value };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
