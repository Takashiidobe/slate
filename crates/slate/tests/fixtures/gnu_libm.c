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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn feenableexcept(__excepts: i32) -> i32;
// CHECK-NEXT:     fn fedisableexcept(__excepts: i32) -> i32;
// CHECK-NEXT:     fn fegetexcept() -> i32;
// CHECK-NEXT:     fn cos(__x: f64) -> f64;
// CHECK-NEXT:     fn sin(__x: f64) -> f64;
// CHECK-NEXT:     fn sincos(__x: f64, __sinx: *mut f64, __cosx: *mut f64);
// CHECK-NEXT:     fn exp10(__x: f64) -> f64;
// CHECK-NEXT:     fn fabs(__x: f64) -> f64;
// CHECK-NEXT:     fn drem(__x: f64, __y: f64) -> f64;
// CHECK-NEXT:     fn significand(__x: f64) -> f64;
// CHECK-NEXT:     fn j0([[V_0:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn j1([[V_1:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn jn([[V_2:__v[0-9]+]]: i32, [[V_3:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn y0([[V_4:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn y1([[V_5:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn yn([[V_6:__v[0-9]+]]: i32, [[V_7:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn lgamma([[V_8:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn lgamma_r([[V_9:__v[0-9]+]]: f64, __signgamp: *mut i32) -> f64;
// CHECK-NEXT:     fn scalb(__x: f64, __n: f64) -> f64;
// CHECK-NEXT:     fn cosf(__x: f32) -> f32;
// CHECK-NEXT:     fn sinf(__x: f32) -> f32;
// CHECK-NEXT:     fn sincosf(__x: f32, __sinx: *mut f32, __cosx: *mut f32);
// CHECK-NEXT:     fn exp10f(__x: f32) -> f32;
// CHECK-NEXT:     fn fabsf(__x: f32) -> f32;
// CHECK-NEXT:     fn dremf(__x: f32, __y: f32) -> f32;
// CHECK-NEXT:     fn significandf(__x: f32) -> f32;
// CHECK-NEXT:     fn j0f([[V_10:__v[0-9]+]]: f32) -> f32;
// CHECK-NEXT:     fn y0f([[V_11:__v[0-9]+]]: f32) -> f32;
// CHECK-NEXT:     fn scalbf(__x: f32, __n: f32) -> f32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_exp10l__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fabsl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_sincosl__rv_f80_pf80_pf80(
// CHECK-NEXT:         _0: LongDouble,
// CHECK-NEXT:         _1: *mut LongDouble,
// CHECK-NEXT:         _2: *mut LongDouble,
// CHECK-NEXT:     );
// CHECK-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_sincos_extensions() -> i32 {
// CHECK-NEXT:     let mut sine: f64 = 0.0f64;
// CHECK-NEXT:     let mut cosine: f64 = 0.0f64;
// CHECK-NEXT:     let mut sine_f: f32 = 0.0f32;
// CHECK-NEXT:     let mut cosine_f: f32 = 0.0f32;
// CHECK-NEXT:     let mut sine_l: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let mut cosine_l: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sincos(0.5f64, std::ptr::addr_of_mut!(sine), std::ptr::addr_of_mut!(cosine))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sincosf(0.5f32, std::ptr::addr_of_mut!(sine_f), std::ptr::addr_of_mut!(cosine_f))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_sincosl__rv_f80_pf80_pf80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]),
// CHECK-NEXT:             std::ptr::addr_of_mut!(sine_l),
// CHECK-NEXT:             std::ptr::addr_of_mut!(cosine_l),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = ((unsafe { fabs(sine - sin(0.5f64)) }) == 0.0f64) as i32;
// CHECK-NEXT:     total += [[V_12]];
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = ((unsafe { fabs(cosine - cos(0.5f64)) }) == 0.0f64) as i32;
// CHECK-NEXT:     total += [[V_13]];
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: i32 = ((unsafe { fabsf(sine_f - sinf(0.5f32)) }) == 0.0f32) as i32;
// CHECK-NEXT:     total += [[V_14]];
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: i32 = ((unsafe { fabsf(cosine_f - cosf(0.5f32)) }) == 0.0f32) as i32;
// CHECK-NEXT:     total += [[V_15]];
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         __slate_fabsl__rf80_f80(
// CHECK-NEXT:             sine_l
// CHECK-NEXT:                 - __slate_sinl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63])),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32;
// CHECK-NEXT:     total += [[V_16]];
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         __slate_fabsl__rf80_f80(
// CHECK-NEXT:             cosine_l
// CHECK-NEXT:                 - __slate_cosl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63])),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32;
// CHECK-NEXT:     total += [[V_17]];
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_exponential_extensions() -> i32 {
// CHECK-NEXT:     let mut sign: i32 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = ((unsafe { exp10(2.0f64) }) == 100.0f64) as i32;
// CHECK-NEXT:     total += [[V_18]];
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: i32 = ((unsafe { exp10f(2.0f32) }) == 100.0f32) as i32;
// CHECK-NEXT:     total += [[V_19]];
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         __slate_exp10l__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]))
// CHECK-NEXT:     }) == LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64])) as i32;
// CHECK-NEXT:     total += [[V_20]];
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: i32 = ((unsafe { lgamma_r(0.5f64, std::ptr::addr_of_mut!(sign)) })
// CHECK-NEXT:         == unsafe { lgamma(0.5f64) }) as i32;
// CHECK-NEXT:     total += [[V_21]];
// CHECK-NEXT:     total += (sign == 1) as i32;
// CHECK-NEXT:     let [[V_22:__v[0-9]+]]: i32 = ((unsafe { significand(12.0f64) }) == 1.5f64) as i32;
// CHECK-NEXT:     total += [[V_22]];
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: i32 = ((unsafe { significandf(12.0f32) }) == 1.5f32) as i32;
// CHECK-NEXT:     total += [[V_23]];
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: i32 = ((unsafe { drem(7.0f64, 3.0f64) }) == 1.0f64) as i32;
// CHECK-NEXT:     total += [[V_24]];
// CHECK-NEXT:     let [[V_25:__v[0-9]+]]: i32 = ((unsafe { dremf(7.0f32, 3.0f32) }) == 1.0f32) as i32;
// CHECK-NEXT:     total += [[V_25]];
// CHECK-NEXT:     let [[V_26:__v[0-9]+]]: i32 = ((unsafe { scalb(1.5f64, 3.0f64) }) == 12.0f64) as i32;
// CHECK-NEXT:     total += [[V_26]];
// CHECK-NEXT:     let [[V_27:__v[0-9]+]]: i32 = ((unsafe { scalbf(1.5f32, 3.0f32) }) == 12.0f32) as i32;
// CHECK-NEXT:     total += [[V_27]];
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_bessel_extensions() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut [[V_28:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { j0(1.0f64) }) == unsafe { j0(1.0f64) } {
// CHECK-NEXT:         [[V_28]] = (unsafe { j0(1.0f64) }) > 0.7f64;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_28]] = false;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_29:__v[0-9]+]]: i32 = [[V_28]] as i32;
// CHECK-NEXT:     total += [[V_29]];
// CHECK-NEXT:     let [[V_30:__v[0-9]+]]: i32 = ((unsafe { j1(1.0f64) }) > 0.4f64) as i32;
// CHECK-NEXT:     total += [[V_30]];
// CHECK-NEXT:     let [[V_31:__v[0-9]+]]: i32 = ((unsafe { jn(2i32, 1.0f64) }) > 0.1f64) as i32;
// CHECK-NEXT:     total += [[V_31]];
// CHECK-NEXT:     let [[V_32:__v[0-9]+]]: i32 = ((unsafe { y0(1.0f64) }) > 0.08f64) as i32;
// CHECK-NEXT:     total += [[V_32]];
// CHECK-NEXT:     let [[V_33:__v[0-9]+]]: i32 = ((unsafe { y1(1.0f64) }) < 0.0f64) as i32;
// CHECK-NEXT:     total += [[V_33]];
// CHECK-NEXT:     let [[V_34:__v[0-9]+]]: i32 = ((unsafe { yn(2i32, 1.0f64) }) < 0.0f64) as i32;
// CHECK-NEXT:     total += [[V_34]];
// CHECK-NEXT:     let [[V_35:__v[0-9]+]]: i32 = ((unsafe { j0f(1.0f32) }) > 0.7f32) as i32;
// CHECK-NEXT:     total += [[V_35]];
// CHECK-NEXT:     let [[V_36:__v[0-9]+]]: i32 = ((unsafe { y0f(1.0f32) }) > 0.08f32) as i32;
// CHECK-NEXT:     total += [[V_36]];
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_fenv_extensions() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_37:__v[0-9]+]]: i32 = ((unsafe { feenableexcept(4i32) }) != -1) as i32;
// CHECK-NEXT:     total += [[V_37]];
// CHECK-NEXT:     let [[V_38:__v[0-9]+]]: i32 = ((unsafe { fegetexcept() }) & 4 != 0) as i32;
// CHECK-NEXT:     total += [[V_38]];
// CHECK-NEXT:     let [[V_39:__v[0-9]+]]: i32 = ((unsafe { fedisableexcept(4i32) }) != -1) as i32;
// CHECK-NEXT:     total += [[V_39]];
// CHECK-NEXT:     let [[V_40:__v[0-9]+]]: i32 = ((unsafe { fegetexcept() }) & 4 == 0) as i32;
// CHECK-NEXT:     total += [[V_40]];
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_constant_extensions() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_41:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_41]]
// CHECK-NEXT:         + ((__slate_f80_to_f64(LongDouble([53, 194, 104, 33, 162, 218, 15, 201, 0, 64]))
// CHECK-NEXT:             == 3.141592653589793f64) as i32);
// CHECK-NEXT:     let [[V_42:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_42]]
// CHECK-NEXT:         + ((__slate_f80_to_f64(LongDouble([155, 74, 187, 162, 88, 84, 248, 173, 0, 64]))
// CHECK-NEXT:             == 2.718281828459045f64) as i32);
// CHECK-NEXT:     let [[V_43:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_43]]
// CHECK-NEXT:         + ((__slate_f80_to_f64(
// CHECK-NEXT:             LongDouble([132, 100, 222, 249, 51, 243, 4, 181, 255, 63]),
// CHECK-NEXT:         ) == 1.4142135623730951f64) as i32);
// CHECK-NEXT:     let [[V_44:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_44]]
// CHECK-NEXT:         + ((__slate_f80_to_f64(
// CHECK-NEXT:             LongDouble([172, 121, 207, 209, 247, 23, 114, 177, 254, 63]),
// CHECK-NEXT:         ) == 0.6931471805599453f64) as i32);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", gnu_sincos_extensions(), gnu_exponential_extensions(),
// CHECK-NEXT:         gnu_bessel_extensions(), gnu_fenv_extensions(), gnu_constant_extensions()
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
