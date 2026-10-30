#include <stdio.h>

#define INF __builtin_inff()
#define NAN __builtin_nanf("")

static void show_d(const char *name, _Complex double z) {
  double re = __real__ z, im = __imag__ z;
  printf("%s ", name);
  if (__builtin_isnan(re))
    printf("nan ");
  else
    printf("%a ", re);
  if (__builtin_isnan(im))
    printf("nan\n");
  else
    printf("%a\n", im);
}

static void show_ld(const char *name, _Complex long double z) {
  long double re = __real__ z, im = __imag__ z;
  printf("%s ", name);
  if (__builtin_isnan(re))
    printf("nan ");
  else
    printf("%.21Lg ", re);
  if (__builtin_isnan(im))
    printf("nan\n");
  else
    printf("%.21Lg\n", im);
}

static _Complex float f_add = __builtin_complex(1.5f, 2.0f) + __builtin_complex(0.25f, -3.0f);
static _Complex float f_mul = __builtin_complex(1.0f, 2.0f) * __builtin_complex(3.0f, 4.0f);
static _Complex float f_div = __builtin_complex(1.0f, 1.0f) / __builtin_complex(0.0f, NAN);
static _Complex float f_real_div = 2.0f / __builtin_complex(1.0f, 1.0f);

static _Complex double d_sub = __builtin_complex(1.0, 2.0) - 0.5;
static _Complex double d_rsub = 0.5 - __builtin_complex(1.0, 2.0);
static _Complex double d_mul = __builtin_complex(0.1, 0.2) * __builtin_complex(0.3, 0.7);
static _Complex double d_scale = __builtin_complex(1.0, 3.0) * 2.0;
static _Complex double d_div = __builtin_complex(1.0, 2.0) / __builtin_complex(3.0, 4.0);
static _Complex double d_tiny = __builtin_complex(1.0, 1.0) / __builtin_complex(1e-300, 1e-300);
static _Complex double d_zero = __builtin_complex(1.0, 1.0) / __builtin_complex(0.0, 0.0);
static _Complex double d_inf_num = __builtin_complex((double)INF, 1.0) / __builtin_complex(1.0, 1.0);
static _Complex double d_inf_den = __builtin_complex(1.0, 1.0) / __builtin_complex((double)INF, 1.0);
static _Complex double d_inf_mul = __builtin_complex((double)INF, (double)NAN) * __builtin_complex(1.0, 1.0);
static _Complex double d_halve = __builtin_complex(3.0, -1.0) / 2.0;
static _Complex double d_neg = -__builtin_complex(1.0, 2.0);
static _Complex double d_conj = ~__builtin_complex(1.0, 2.0);

static _Complex long double ld_mul = __builtin_complex(1.0L, 2.0L) * __builtin_complex(0.1L, 0.3L);
static _Complex long double ld_div = __builtin_complex(1.0L, 2.0L) / __builtin_complex(3.0L, 4.0L);
static _Complex long double ld_nan = __builtin_complex(1.0L, 1.0L) / __builtin_complex(0.0L, (long double)NAN);

int main(void) {
  volatile _Complex double num = __builtin_complex(1.0, 1.0);
  volatile _Complex double den = __builtin_complex(0.0, (double)NAN);
  volatile _Complex float numf = __builtin_complex(1.0f, 1.0f);
  volatile _Complex float denf = __builtin_complex(0.0f, NAN);
  show_d("f_add", f_add);
  show_d("f_mul", f_mul);
  show_d("f_div", f_div);
  show_d("f_real_div", f_real_div);
  show_d("d_sub", d_sub);
  show_d("d_rsub", d_rsub);
  show_d("d_mul", d_mul);
  show_d("d_scale", d_scale);
  show_d("d_div", d_div);
  show_d("d_tiny", d_tiny);
  show_d("d_zero", d_zero);
  show_d("d_inf_num", d_inf_num);
  show_d("d_inf_den", d_inf_den);
  show_d("d_inf_mul", d_inf_mul);
  show_d("d_halve", d_halve);
  show_d("d_neg", d_neg);
  show_d("d_conj", d_conj);
  show_d("rt_div", num / den);
  show_d("rt_divf", numf / denf);
  show_ld("ld_mul", ld_mul);
  show_ld("ld_div", ld_div);
  show_ld("ld_nan", ld_nan);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
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
// CHECK-NEXT:     safe fn __slate_cf80_add(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_sub(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_mul(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_mul_basic(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div_basic(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div_improved(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_neg(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
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
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *const i8, _1: LongDouble) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
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
// CHECK-NEXT: fn show_d(name: *const i8, z: num_complex::Complex<f64>) {
// CHECK-NEXT:     let re: f64 = z.re;
// CHECK-NEXT:     let im: f64 = z.im;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s ".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if re.is_nan() {
// CHECK-NEXT:         print!("nan ");
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%a ".as_ptr(), re) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if im.is_nan() {
// CHECK-NEXT:         println!("nan");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%a\n".as_ptr(), im) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn show_ld(name: *const i8, z: num_complex::Complex<LongDouble>) {
// CHECK-NEXT:     let re: LongDouble = z.re;
// CHECK-NEXT:     let im: LongDouble = z.im;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s ".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if __slate_f80_is_fp_class(re, 3) {
// CHECK-NEXT:         print!("nan ");
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { __slate_printf__ri32_pi8_f80(c"%.21Lg ".as_ptr(), re) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if __slate_f80_is_fp_class(im, 3) {
// CHECK-NEXT:         println!("nan");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { __slate_printf__ri32_pi8_f80(c"%.21Lg\n".as_ptr(), im) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut f_add: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:     re: 1.75f32,
// CHECK-NEXT:     im: -1.0f32,
// CHECK-NEXT: };
// CHECK-NEXT: static mut f_mul: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:     re: -5.0f32,
// CHECK-NEXT:     im: 10.0f32,
// CHECK-NEXT: };
// CHECK-NEXT: static mut f_div: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:     re: f32::from_bits(0x7fc00000),
// CHECK-NEXT:     im: f32::from_bits(0x7fc00000),
// CHECK-NEXT: };
// CHECK-NEXT: static mut f_real_div: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:     re: 1.0f32,
// CHECK-NEXT:     im: -1.0f32,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_sub: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 0.5f64,
// CHECK-NEXT:     im: 2.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_rsub: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: -0.5f64,
// CHECK-NEXT:     im: -2.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_mul: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: -0.10999999999999999f64,
// CHECK-NEXT:     im: 0.13f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_scale: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 2.0f64,
// CHECK-NEXT:     im: 6.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_div: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 0.44f64,
// CHECK-NEXT:     im: 0.08f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_tiny: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 1e300f64,
// CHECK-NEXT:     im: 0.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_zero: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: f64::INFINITY,
// CHECK-NEXT:     im: f64::INFINITY,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_inf_num: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: f64::INFINITY,
// CHECK-NEXT:     im: f64::NEG_INFINITY,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_inf_den: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 0.0f64,
// CHECK-NEXT:     im: 0.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_inf_mul: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: f64::INFINITY,
// CHECK-NEXT:     im: f64::INFINITY,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_halve: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 1.5f64,
// CHECK-NEXT:     im: -0.5f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_neg: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: -1.0f64,
// CHECK-NEXT:     im: -2.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut d_conj: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 1.0f64,
// CHECK-NEXT:     im: -2.0f64,
// CHECK-NEXT: };
// CHECK-NEXT: static mut ld_mul: num_complex::Complex<LongDouble> = num_complex::Complex {
// CHECK-NEXT:     re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 191]),
// CHECK-NEXT:     im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]),
// CHECK-NEXT: };
// CHECK-NEXT: static mut ld_div: num_complex::Complex<LongDouble> = num_complex::Complex {
// CHECK-NEXT:     re: LongDouble([174, 71, 225, 122, 20, 174, 71, 225, 253, 63]),
// CHECK-NEXT:     im: LongDouble([10, 215, 163, 112, 61, 10, 215, 163, 251, 63]),
// CHECK-NEXT: };
// CHECK-NEXT: static mut ld_nan: num_complex::Complex<LongDouble> = num_complex::Complex {
// CHECK-NEXT:     re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// CHECK-NEXT:     im: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let num: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: 1.0f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let den: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 0.0f64,
// CHECK-NEXT:         im: f32::from_bits(0x7fc00000) as f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let numf: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:         re: 1.0f32,
// CHECK-NEXT:         im: 1.0f32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let denf: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:         re: 0.0f32,
// CHECK-NEXT:         im: f32::from_bits(0x7fc00000),
// CHECK-NEXT:     };
// CHECK-NEXT:     show_d(
// CHECK-NEXT:         c"f_add".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = unsafe { f_add };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_0]].re as f64,
// CHECK-NEXT:                 im: [[T_0]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     show_d(
// CHECK-NEXT:         c"f_mul".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = unsafe { f_mul };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]].re as f64,
// CHECK-NEXT:                 im: [[T_1]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     show_d(
// CHECK-NEXT:         c"f_div".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = unsafe { f_div };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_2]].re as f64,
// CHECK-NEXT:                 im: [[T_2]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     show_d(
// CHECK-NEXT:         c"f_real_div".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = unsafe { f_real_div };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_3]].re as f64,
// CHECK-NEXT:                 im: [[T_3]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     show_d(c"d_sub".as_ptr(), unsafe { d_sub });
// CHECK-NEXT:     show_d(c"d_rsub".as_ptr(), unsafe { d_rsub });
// CHECK-NEXT:     show_d(c"d_mul".as_ptr(), unsafe { d_mul });
// CHECK-NEXT:     show_d(c"d_scale".as_ptr(), unsafe { d_scale });
// CHECK-NEXT:     show_d(c"d_div".as_ptr(), unsafe { d_div });
// CHECK-NEXT:     show_d(c"d_tiny".as_ptr(), unsafe { d_tiny });
// CHECK-NEXT:     show_d(c"d_zero".as_ptr(), unsafe { d_zero });
// CHECK-NEXT:     show_d(c"d_inf_num".as_ptr(), unsafe { d_inf_num });
// CHECK-NEXT:     show_d(c"d_inf_den".as_ptr(), unsafe { d_inf_den });
// CHECK-NEXT:     show_d(c"d_inf_mul".as_ptr(), unsafe { d_inf_mul });
// CHECK-NEXT:     show_d(c"d_halve".as_ptr(), unsafe { d_halve });
// CHECK-NEXT:     show_d(c"d_neg".as_ptr(), unsafe { d_neg });
// CHECK-NEXT:     show_d(c"d_conj".as_ptr(), unsafe { d_conj });
// CHECK-NEXT:     show_d(
// CHECK-NEXT:         c"rt_div".as_ptr(),
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num)) }
// CHECK-NEXT:             .c_div(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den)) }, false),
// CHECK-NEXT:     );
// CHECK-NEXT:     show_d(
// CHECK-NEXT:         c"rt_divf".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(numf)) }
// CHECK-NEXT:                 .c_div(
// CHECK-NEXT:                     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(denf)) },
// CHECK-NEXT:                     false,
// CHECK-NEXT:                 );
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_4]].re as f64,
// CHECK-NEXT:                 im: [[T_4]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     show_ld(c"ld_mul".as_ptr(), unsafe { ld_mul });
// CHECK-NEXT:     show_ld(c"ld_div".as_ptr(), unsafe { ld_div });
// CHECK-NEXT:     show_ld(c"ld_nan".as_ptr(), unsafe { ld_nan });
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
