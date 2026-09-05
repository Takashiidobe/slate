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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-LOWERING-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-LOWERING-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-LOWERING-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut z: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 1.0, im: 2.0 };
// COMMON-LOWERING-NEXT:     z = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 7.0;
// COMMON-LOWERING-NEXT:     z.re = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 11.0;
// COMMON-LOWERING-NEXT:     z.im = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = pick(std::ptr::addr_of_mut!(z.re));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = pick(std::ptr::addr_of_mut!(z.im));
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pick({{arg[0-9]+}}: *mut f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 5.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-REWRITES-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-REWRITES-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-REWRITES-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut z: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// COMMON-REWRITES-NEXT:     z = num_complex::Complex { re: 1.0, im: 2.0 };
// COMMON-REWRITES-NEXT:     z.re = 7.0;
// COMMON-REWRITES-NEXT:     z.im = 11.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.re)) });
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.im)) });
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = z;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}.re as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = z;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}.im as i32) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}} as i32) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}} as i32) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pick({{arg[0-9]+}}: &mut f64) -> f64 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *({{arg[0-9]+}} as *mut f64) = (unsafe { *({{arg[0-9]+}} as *mut f64) }) + 5.0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { *({{arg[0-9]+}} as *mut f64) }
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
