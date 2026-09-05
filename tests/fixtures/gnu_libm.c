#define _GNU_SOURCE
#include <complex.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>

static int gnu_sincos_extensions(void) {
  double      sine        = 0.0;
  double      cosine      = 0.0;
  float       sine_f      = 0.0f;
  float       cosine_f    = 0.0f;
  long double sine_l      = 0.0L;
  long double cosine_l    = 0.0L;
  int         total       = 0;

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
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct LongDouble([u8; 10]);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_add(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_div(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_neg(self)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     None
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn sincos(_0: f64, _1: *mut f64, _2: *mut f64);
// LOWERING-NEXT:     fn sincosf(_0: f32, _1: *mut f32, _2: *mut f32);
// LOWERING-NEXT:     fn sincosl(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// LOWERING-NEXT:     fn sin(_0: f64) -> f64;
// LOWERING-NEXT:     fn cos(_0: f64) -> f64;
// LOWERING-NEXT:     fn sinf(_0: f32) -> f32;
// LOWERING-NEXT:     fn cosf(_0: f32) -> f32;
// LOWERING-NEXT:     fn sinl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn cosl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn exp10(_0: f64) -> f64;
// LOWERING-NEXT:     fn exp10f(_0: f32) -> f32;
// LOWERING-NEXT:     fn exp10l(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn lgamma_r(_0: f64, _1: *mut i32) -> f64;
// LOWERING-NEXT:     fn lgamma(_0: f64) -> f64;
// LOWERING-NEXT:     fn significand(_0: f64) -> f64;
// LOWERING-NEXT:     fn significandf(_0: f32) -> f32;
// LOWERING-NEXT:     fn drem(_0: f64, _1: f64) -> f64;
// LOWERING-NEXT:     fn dremf(_0: f32, _1: f32) -> f32;
// LOWERING-NEXT:     fn scalb(_0: f64, _1: f64) -> f64;
// LOWERING-NEXT:     fn scalbf(_0: f32, _1: f32) -> f32;
// LOWERING-NEXT:     fn j0(_0: f64) -> f64;
// LOWERING-NEXT:     fn j1(_0: f64) -> f64;
// LOWERING-NEXT:     fn jn(_0: i32, _1: f64) -> f64;
// LOWERING-NEXT:     fn y0(_0: f64) -> f64;
// LOWERING-NEXT:     fn y1(_0: f64) -> f64;
// LOWERING-NEXT:     fn yn(_0: i32, _1: f64) -> f64;
// LOWERING-NEXT:     fn j0f(_0: f32) -> f32;
// LOWERING-NEXT:     fn y0f(_0: f32) -> f32;
// LOWERING-NEXT:     fn feenableexcept(_0: i32) -> i32;
// LOWERING-NEXT:     fn fegetexcept() -> i32;
// LOWERING-NEXT:     fn fedisableexcept(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_sincos_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_exponential_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_bessel_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_fenv_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_constant_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_sincos_extensions() -> i32 {
// LOWERING-NEXT:     let mut sine: f64 = 0.0;
// LOWERING-NEXT:     let mut cosine: f64 = 0.0;
// LOWERING-NEXT:     let mut sine_f: f32 = 0.0;
// LOWERING-NEXT:     let mut cosine_f: f32 = 0.0;
// LOWERING-NEXT:     let mut sine_l: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut cosine_l: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     sine = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     cosine = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     sine_f = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     cosine_f = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     sine_l = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     cosine_l = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         sincos(
// LOWERING-NEXT:             {{_v[0-9]+}} as f64,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(sine) as *mut f64,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(cosine) as *mut f64,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         sincosf(
// LOWERING-NEXT:             {{_v[0-9]+}} as f32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(sine_f) as *mut f32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(cosine_f) as *mut f32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         __slate_sincosl__rv_f80_pf80_pf80(
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             std::ptr::addr_of_mut!(sine_l),
// LOWERING-NEXT:             std::ptr::addr_of_mut!(cosine_l),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = sine;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { sin({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.abs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = cosine;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { cos({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.abs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = sine_f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { sinf({{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.abs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = cosine_f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { cosf({{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.abs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = sine_l;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_sinl__rf80_f80({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_abs({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = cosine_l;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_cosl__rf80_f80({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_abs({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_exponential_extensions() -> i32 {
// LOWERING-NEXT:     let mut sign: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     sign = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { exp10({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 100.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { exp10f({{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 100.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_exp10l__rf80_f80({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { lgamma_r({{_v[0-9]+}} as f64, std::ptr::addr_of_mut!(sign) as *mut i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { lgamma({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sign;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 12.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { significand({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 12.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { significandf({{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 7.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { drem({{_v[0-9]+}} as f64, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 7.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { dremf({{_v[0-9]+}} as f32, {{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { scalb({{_v[0-9]+}} as f64, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 12.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { scalbf({{_v[0-9]+}} as f32, {{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 12.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_bessel_extensions() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { j0({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { j0({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { j0({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 0.7;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { j1({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { jn({{_v[0-9]+}} as i32, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { y0({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.08;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { y1({{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { yn({{_v[0-9]+}} as i32, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { j0f({{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.699999988;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { y0f({{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.08;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_fenv_extensions() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { feenableexcept({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fegetexcept() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fedisableexcept({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fegetexcept() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_constant_extensions() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([53, 194, 104, 33, 162, 218, 15, 201, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.141592653589793;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([155, 74, 187, 162, 88, 84, 248, 173, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.718281828459045;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([132, 100, 222, 249, 51, 243, 4, 181, 255, 63]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.4142135623730951;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([172, 121, 207, 209, 247, 23, 114, 177, 254, 63]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.6931471805599453;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     safe fn __slate_cf80_div(
// LOWERING-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_exp10l__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_sincosl__rv_f80_pf80_pf80(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// LOWERING-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
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
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct LongDouble([u8; 10]);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_add(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_div(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_neg(self)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-NEXT:             } else {
// REWRITES-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     None
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn sincos(_0: f64, _1: *mut f64, _2: *mut f64);
// REWRITES-NEXT:     fn sincosf(_0: f32, _1: *mut f32, _2: *mut f32);
// REWRITES-NEXT:     fn sincosl(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// REWRITES-NEXT:     fn sin(_0: f64) -> f64;
// REWRITES-NEXT:     fn cos(_0: f64) -> f64;
// REWRITES-NEXT:     fn sinf(_0: f32) -> f32;
// REWRITES-NEXT:     fn cosf(_0: f32) -> f32;
// REWRITES-NEXT:     fn sinl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn cosl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn exp10(_0: f64) -> f64;
// REWRITES-NEXT:     fn exp10f(_0: f32) -> f32;
// REWRITES-NEXT:     fn exp10l(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn lgamma_r(_0: f64, _1: *mut i32) -> f64;
// REWRITES-NEXT:     fn lgamma(_0: f64) -> f64;
// REWRITES-NEXT:     fn significand(_0: f64) -> f64;
// REWRITES-NEXT:     fn significandf(_0: f32) -> f32;
// REWRITES-NEXT:     fn drem(_0: f64, _1: f64) -> f64;
// REWRITES-NEXT:     fn dremf(_0: f32, _1: f32) -> f32;
// REWRITES-NEXT:     fn scalb(_0: f64, _1: f64) -> f64;
// REWRITES-NEXT:     fn scalbf(_0: f32, _1: f32) -> f32;
// REWRITES-NEXT:     fn j0(_0: f64) -> f64;
// REWRITES-NEXT:     fn j1(_0: f64) -> f64;
// REWRITES-NEXT:     fn jn(_0: i32, _1: f64) -> f64;
// REWRITES-NEXT:     fn y0(_0: f64) -> f64;
// REWRITES-NEXT:     fn y1(_0: f64) -> f64;
// REWRITES-NEXT:     fn yn(_0: i32, _1: f64) -> f64;
// REWRITES-NEXT:     fn j0f(_0: f32) -> f32;
// REWRITES-NEXT:     fn y0f(_0: f32) -> f32;
// REWRITES-NEXT:     fn feenableexcept(_0: i32) -> i32;
// REWRITES-NEXT:     fn fegetexcept() -> i32;
// REWRITES-NEXT:     fn fedisableexcept(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             gnu_sincos_extensions(),
// REWRITES-NEXT:             gnu_exponential_extensions(),
// REWRITES-NEXT:             gnu_bessel_extensions(),
// REWRITES-NEXT:             gnu_fenv_extensions(),
// REWRITES-NEXT:             gnu_constant_extensions(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_sincos_extensions() -> i32 {
// REWRITES-NEXT:     let mut sine: f64 = 0.0;
// REWRITES-NEXT:     let mut cosine: f64 = 0.0;
// REWRITES-NEXT:     let mut sine_f: f32 = 0.0;
// REWRITES-NEXT:     let mut cosine_f: f32 = 0.0;
// REWRITES-NEXT:     let mut sine_l: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut cosine_l: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     sine = 0.0;
// REWRITES-NEXT:     cosine = 0.0;
// REWRITES-NEXT:     sine_f = 0.0;
// REWRITES-NEXT:     cosine_f = 0.0;
// REWRITES-NEXT:     sine_l = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     cosine_l = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         sincos(
// REWRITES-NEXT:             0.5 as f64,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(sine) as *mut f64,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(cosine) as *mut f64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         sincosf(
// REWRITES-NEXT:             0.5 as f32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(sine_f) as *mut f32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(cosine_f) as *mut f32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_sincosl__rv_f80_pf80_pf80(
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             std::ptr::addr_of_mut!(sine_l),
// REWRITES-NEXT:             std::ptr::addr_of_mut!(cosine_l),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = sine;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { sin(0.5 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.abs();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 0.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = cosine;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { cos(0.5 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.abs();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 0.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = sine_f;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { sinf(0.5 as f32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.abs();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 0.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = cosine_f;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { cosf(0.5 as f32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.abs();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 0.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = sine_l;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_sinl__rf80_f80({{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_abs({{_v[0-9]+}} - {{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = cosine_l;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_cosl__rf80_f80({{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_abs({{_v[0-9]+}} - {{_v[0-9]+}});
// REWRITES-NEXT:     {{_v[0-9]+}} + (({{_v[0-9]+}} == LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_exponential_extensions() -> i32 {
// REWRITES-NEXT:     let mut sign: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { exp10(2.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 100.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { exp10f(2.0 as f32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 100.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_exp10l__rf80_f80({{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64])) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { lgamma_r(0.5 as f64, std::ptr::addr_of_mut!(sign) as *mut i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { lgamma(0.5 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + ((sign == 1) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { significand(12.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 1.5) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { significandf(12.0 as f32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 1.5) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { drem(7.0 as f64, 3.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 1.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { dremf(7.0 as f32, 3.0 as f32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 1.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { scalb(1.5 as f64, 3.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 12.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { scalbf(1.5 as f32, 3.0 as f32) };
// REWRITES-NEXT:     {{_v[0-9]+}} + (({{_v[0-9]+}} == 12.0) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_bessel_extensions() -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { j0(1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { j0(1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { j0(1.0 as f64) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > 0.7;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { j1(1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > 0.4) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { jn(2 as i32, 1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > 0.1) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { y0(1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > 0.08) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { y1(1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} < 0.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { yn(2 as i32, 1.0 as f64) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} < 0.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { j0f(1.0 as f32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > 0.699999988) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { y0f(1.0 as f32) };
// REWRITES-NEXT:     {{_v[0-9]+}} + (({{_v[0-9]+}} > 0.08) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_fenv_extensions() -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { feenableexcept(4 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != -1) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fegetexcept() };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} & 4 != 0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fedisableexcept(4 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != -1) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fegetexcept() };
// REWRITES-NEXT:     {{_v[0-9]+}} + (({{_v[0-9]+}} & 4 == 0) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_constant_extensions() -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([53, 194, 104, 33, 162, 218, 15, 201, 0, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 3.141592653589793) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([155, 74, 187, 162, 88, 84, 248, 173, 0, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 2.718281828459045) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([132, 100, 222, 249, 51, 243, 4, 181, 255, 63]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == 1.4142135623730951) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([172, 121, 207, 209, 247, 23, 114, 177, 254, 63]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT:     {{_v[0-9]+}} + (({{_v[0-9]+}} == 0.6931471805599453) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     safe fn __slate_cf80_div(
// REWRITES-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_exp10l__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_sincosl__rv_f80_pf80_pf80(_0: LongDouble, _1: *mut LongDouble, _2: *mut LongDouble);
// REWRITES-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
