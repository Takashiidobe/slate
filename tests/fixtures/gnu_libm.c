#define _GNU_SOURCE
#include <complex.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>

static int gnu_sincos_extensions(void) {
  double      sine     = 0.0;
  double      cosine   = 0.0;
  float       sine_f   = 0.0f;
  float       cosine_f = 0.0f;
  long double sine_l   = 0.0L;
  long double cosine_l = 0.0L;
  int         total    = 0;

  sincos(0.5, &sine, &cosine);
  sincosf(0.5f, &sine_f, &cosine_f);
  sincosl(0.5L, &sine_l, &cosine_l);

  total += fabs(sine - sin(0.5)) == 0.0;
  total += fabs(cosine - cos(0.5)) == 0.0;
  total += fabsf(sine_f - sinf(0.5f)) == 0.0f;
  total += fabsf(cosine_f - cosf(0.5f)) == 0.0f;
  total += fabsl(sine_l - sinl(0.5L)) == 0.0L;
  total += fabsl(cosine_l - cosl(0.5L)) == 0.0L;
  return total;
}

static int gnu_exponential_extensions(void) {
  int sign  = 0;
  int total = 0;

  total += exp10(2.0) == 100.0;
  total += exp10f(2.0f) == 100.0f;
  total += exp10l(2.0L) == 100.0L;
  total += lgamma_r(0.5, &sign) == lgamma(0.5);
  total += sign == 1;
  total += significand(12.0) == 1.5;
  total += significandf(12.0f) == 1.5f;
  total += drem(7.0, 3.0) == 1.0;
  total += dremf(7.0f, 3.0f) == 1.0f;
  total += scalb(1.5, 3.0) == 12.0;
  total += scalbf(1.5f, 3.0f) == 12.0f;
  return total;
}

static int gnu_bessel_extensions(void) {
  int total = 0;

  total += j0(1.0) == j0(1.0) && j0(1.0) > 0.7;
  total += j1(1.0) > 0.4;
  total += jn(2, 1.0) > 0.1;
  total += y0(1.0) > 0.08;
  total += y1(1.0) < 0.0;
  total += yn(2, 1.0) < 0.0;
  total += j0f(1.0f) > 0.7f;
  total += y0f(1.0f) > 0.08f;
  return total;
}

static int gnu_fenv_extensions(void) {
  int total = 0;

  total += feenableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) != 0;
  total += fedisableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) == 0;
  return total;
}

static int gnu_constant_extensions(void) {
  int total = 0;

  total += (double)M_PIl == M_PI;
  total += (double)M_El == M_E;
  total += (double)M_SQRT2l == M_SQRT2;
  total += (double)M_LN2l == M_LN2;
  return total;
}

int main(void) {
  printf("%d %d %d %d %d\n", gnu_sincos_extensions(),
         gnu_exponential_extensions(), gnu_bessel_extensions(),
         gnu_fenv_extensions(), gnu_constant_extensions());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f128)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
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
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn sincos(_0: f64, _1: *mut f64, _2: *mut f64);
// COMMON-LOWERING-NEXT:     fn sincosf(_0: f32, _1: *mut f32, _2: *mut f32);
// COMMON-LOWERING-NEXT:     fn sin(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn cos(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn sinf(_0: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn cosf(_0: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn exp10(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn exp10f(_0: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn lgamma_r(_0: f64, _1: *mut i32) -> f64;
// COMMON-LOWERING-NEXT:     fn lgamma(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn significand(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn significandf(_0: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn drem(_0: f64, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn dremf(_0: f32, _1: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn scalb(_0: f64, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn scalbf(_0: f32, _1: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn j0(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn j1(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn jn(_0: i32, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn y0(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn y1(_0: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn yn(_0: i32, _1: f64) -> f64;
// COMMON-LOWERING-NEXT:     fn j0f(_0: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn y0f(_0: f32) -> f32;
// COMMON-LOWERING-NEXT:     fn feenableexcept(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn fegetexcept() -> i32;
// COMMON-LOWERING-NEXT:     fn fedisableexcept(_0: i32) -> i32;
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_sincos_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_exponential_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_bessel_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_fenv_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_constant_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: fn gnu_sincos_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut sine: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut cosine: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut sine_f: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let mut cosine_f: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     sine = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     cosine = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     sine_f = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     cosine_f = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     sine_l = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     cosine_l = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         sincos(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as f64,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(sine) as *mut f64,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(cosine) as *mut f64,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.5;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         sincosf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as f32,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(sine_f) as *mut f32,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(cosine_f) as *mut f32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = sine;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sin({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.abs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = cosine;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cos({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.abs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = sine_f;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { sinf({{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.abs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = cosine_f;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { cosf({{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.abs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn gnu_exponential_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut sign: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     sign = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp10({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 100.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { exp10f({{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 100.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { lgamma_r({{__v[0-9]+}} as f64, std::ptr::addr_of_mut!(sign) as *mut i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { lgamma({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sign;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 12.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { significand({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 12.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { significandf({{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 7.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { drem({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 7.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { dremf({{__v[0-9]+}} as f32, {{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { scalb({{__v[0-9]+}} as f64, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 12.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { scalbf({{__v[0-9]+}} as f32, {{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 12.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn gnu_bessel_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { j0({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { j0({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { j0({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 0.7;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { j1({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { jn({{__v[0-9]+}} as i32, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { y0({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.08;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { y1({{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { yn({{__v[0-9]+}} as i32, {{__v[0-9]+}} as f64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { j0f({{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.699999988;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { y0f({{__v[0-9]+}} as f32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.08;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn gnu_fenv_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { feenableexcept({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fegetexcept() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fedisableexcept({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fegetexcept() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn gnu_constant_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.141592653589793;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.718281828459045;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.4142135623730951;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.6931471805599453;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn sincosl(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// LOWERING-X86_64-GNU-NEXT:     fn sinl(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn cosl(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn exp10l(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut sine_l: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut cosine_l: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:         __slate_sincosl__rv_f80_pf80_pf80(
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(sine_l),
// LOWERING-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(cosine_l),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = sine_l;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sinl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = cosine_l;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_cosl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_exp10l__rf80_f80({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([53, 194, 104, 33, 162, 218, 15, 201, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([155, 74, 187, 162, 88, 84, 248, 173, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([132, 100, 222, 249, 51, 243, 4, 181, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([172, 121, 207, 209, 247, 23, 114, 177, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_exp10l__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_sincosl__rv_f80_pf80_pf80(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// LOWERING-X86_64-GNU-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     fn sincosl(_0: f128, _1: *mut f128, _2: *mut f128);
// LOWERING-AARCH64-GNU-NEXT:     fn sinl(_0: f128) -> f128;
// LOWERING-AARCH64-GNU-NEXT:     fn cosl(_0: f128) -> f128;
// LOWERING-AARCH64-GNU-NEXT:     fn exp10l(_0: f128) -> f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut sine_l: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let mut cosine_l: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// LOWERING-AARCH64-GNU-NEXT:         sincosl(
// LOWERING-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as f128,
// LOWERING-AARCH64-GNU-NEXT:             std::ptr::addr_of_mut!(sine_l) as *mut f128,
// LOWERING-AARCH64-GNU-NEXT:             std::ptr::addr_of_mut!(cosine_l) as *mut f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = sine_l;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { sinl({{__v[0-9]+}} as f128) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = cosine_l;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { cosl({{__v[0-9]+}} as f128) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { exp10l({{__v[0-9]+}} as f128) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.000000e+02f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.1415926535897932384626433832795028f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.71828182845904523536028747135266231f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.41421356237309504880168872420969798f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.693147180559945309417232121458176575f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(f128)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
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
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn sincos(_0: f64, _1: *mut f64, _2: *mut f64);
// COMMON-REWRITES-NEXT:     fn sincosf(_0: f32, _1: *mut f32, _2: *mut f32);
// COMMON-REWRITES-NEXT:     fn sin(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn cos(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn sinf(_0: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn cosf(_0: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn exp10(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn exp10f(_0: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn lgamma_r(_0: f64, _1: *mut i32) -> f64;
// COMMON-REWRITES-NEXT:     fn lgamma(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn significand(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn significandf(_0: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn drem(_0: f64, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn dremf(_0: f32, _1: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn scalb(_0: f64, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn scalbf(_0: f32, _1: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn j0(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn j1(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn jn(_0: i32, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn y0(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn y1(_0: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn yn(_0: i32, _1: f64) -> f64;
// COMMON-REWRITES-NEXT:     fn j0f(_0: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn y0f(_0: f32) -> f32;
// COMMON-REWRITES-NEXT:     fn feenableexcept(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn fegetexcept() -> i32;
// COMMON-REWRITES-NEXT:     fn fedisableexcept(_0: i32) -> i32;
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             gnu_sincos_extensions(),
// COMMON-REWRITES-NEXT:             gnu_exponential_extensions(),
// COMMON-REWRITES-NEXT:             gnu_bessel_extensions(),
// COMMON-REWRITES-NEXT:             gnu_fenv_extensions(),
// COMMON-REWRITES-NEXT:             gnu_constant_extensions(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: fn gnu_sincos_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut sine: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut cosine: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut sine_f: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let mut cosine_f: f32 = 0.0;
// COMMON-REWRITES-NEXT:     sine = 0.0;
// COMMON-REWRITES-NEXT:     cosine = 0.0;
// COMMON-REWRITES-NEXT:     sine_f = 0.0;
// COMMON-REWRITES-NEXT:     cosine_f = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         sincos(
// COMMON-REWRITES-NEXT:             0.5 as f64,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(sine) as *mut f64,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(cosine) as *mut f64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         sincosf(
// COMMON-REWRITES-NEXT:             0.5 as f32,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(sine_f) as *mut f32,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(cosine_f) as *mut f32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = sine;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { sin(0.5 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.abs();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = cosine;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { cos(0.5 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.abs();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = sine_f;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { sinf(0.5 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.abs();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = cosine_f;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { cosf(0.5 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.abs();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0.0) as i32);
// COMMON-REWRITES-NEXT: fn gnu_exponential_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut sign: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { exp10(2.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 100.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { exp10f(2.0 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 100.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { lgamma_r(0.5 as f64, std::ptr::addr_of_mut!(sign) as *mut i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { lgamma(0.5 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == {{__v[0-9]+}}) as i32) + ((sign == 1) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { significand(12.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 1.5) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { significandf(12.0 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 1.5) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { drem(7.0 as f64, 3.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 1.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { dremf(7.0 as f32, 3.0 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 1.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { scalb(1.5 as f64, 3.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 12.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { scalbf(1.5 as f32, 3.0 as f32) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == 12.0) as i32)
// COMMON-REWRITES-NEXT: fn gnu_bessel_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { j0(1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { j0(1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { j0(1.0 as f64) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > 0.7;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { j1(1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0.4) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { jn(2 as i32, 1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0.1) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { y0(1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0.08) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { y1(1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} < 0.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { yn(2 as i32, 1.0 as f64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} < 0.0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { j0f(1.0 as f32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0.699999988) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { y0f(1.0 as f32) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} > 0.08) as i32)
// COMMON-REWRITES-NEXT: fn gnu_fenv_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != -1) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fegetexcept() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != -1) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fegetexcept() };
// COMMON-REWRITES-NEXT: fn gnu_constant_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn sincosl(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// REWRITES-X86_64-GNU-NEXT:     fn sinl(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn cosl(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn exp10l(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut sine_l: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut cosine_l: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     sine_l = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     cosine_l = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:         __slate_sincosl__rv_f80_pf80_pf80(
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(sine_l),
// REWRITES-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(cosine_l),
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = sine_l;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sinl__rf80_f80({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}} - {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = cosine_l;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_cosl__rf80_f80({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}} - {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32)
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_exp10l__rf80_f80({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64])) as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { feenableexcept(4 as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} & 4 != 0) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fedisableexcept(4 as i32) };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} & 4 == 0) as i32)
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([53, 194, 104, 33, 162, 218, 15, 201, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 3.141592653589793) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([155, 74, 187, 162, 88, 84, 248, 173, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 2.718281828459045) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([132, 100, 222, 249, 51, 243, 4, 181, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 1.4142135623730951) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([172, 121, 207, 209, 247, 23, 114, 177, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == 0.6931471805599453) as i32)
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_exp10l__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_sincosl__rv_f80_pf80_pf80(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// REWRITES-X86_64-GNU-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     fn sincosl(_0: f128, _1: *mut f128, _2: *mut f128);
// REWRITES-AARCH64-GNU-NEXT:     fn sinl(_0: f128) -> f128;
// REWRITES-AARCH64-GNU-NEXT:     fn cosl(_0: f128) -> f128;
// REWRITES-AARCH64-GNU-NEXT:     fn exp10l(_0: f128) -> f128;
// REWRITES-AARCH64-GNU-NEXT:     let mut sine_l: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let mut cosine_l: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     sine_l = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     cosine_l = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// REWRITES-AARCH64-GNU-NEXT:         sincosl(
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as f128,
// REWRITES-AARCH64-GNU-NEXT:             std::ptr::addr_of_mut!(sine_l) as *mut f128,
// REWRITES-AARCH64-GNU-NEXT:             std::ptr::addr_of_mut!(cosine_l) as *mut f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = sine_l;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { sinl({{__v[0-9]+}} as f128) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == {{__v[0-9]+}}) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = cosine_l;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e-01f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { cosl({{__v[0-9]+}} as f128) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { exp10l({{__v[0-9]+}} as f128) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.000000e+02f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == {{__v[0-9]+}}) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { feenableexcept(2 as i32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} & 2 != 0) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fedisableexcept(2 as i32) };
// REWRITES-AARCH64-GNU-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} & 2 == 0) as i32)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.1415926535897932384626433832795028f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ((({{__v[0-9]+}} as f64) == 3.141592653589793) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.71828182845904523536028747135266231f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ((({{__v[0-9]+}} as f64) == 2.718281828459045) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.41421356237309504880168872420969798f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ((({{__v[0-9]+}} as f64) == 1.4142135623730951) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.693147180559945309417232121458176575f128;
// REWRITES-AARCH64-GNU-NEXT:     {{__v[0-9]+}} + ((({{__v[0-9]+}} as f64) == 0.6931471805599453) as i32)
// SLATE-FILECHECK-END rewrites-aarch64-gnu
