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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT: fn pick({{arg[0-9]+}}: *mut f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 5.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut z: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 1.0, im: 2.0 };
// LOWERING-NEXT:     z = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 7.0;
// LOWERING-NEXT:     z.re = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 11.0;
// LOWERING-NEXT:     z.im = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = pick(std::ptr::addr_of_mut!(z.re));
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = pick(std::ptr::addr_of_mut!(z.im));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-NEXT: fn pick({{arg[0-9]+}}: &mut f64) -> f64 {
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 5.0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *({{arg[0-9]+}} as *mut f64) = (unsafe { *({{arg[0-9]+}} as *mut f64) }) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: return unsafe { *({{arg[0-9]+}} as *mut f64) };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut z: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: z = num_complex::Complex { re: 1.0, im: 2.0 };
// REWRITES-NEXT: z.re = 7.0;
// REWRITES-NEXT: z.im = 11.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.re)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.im)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<f64> = z;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}.re as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<f64> = z;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}.im as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
