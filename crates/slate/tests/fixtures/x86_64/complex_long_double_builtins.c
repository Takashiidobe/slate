#include <stdio.h>

struct S {
  _Complex unsigned short x;
  _Complex char           y;
};

struct S gs = {100 + 200i, 3 - 4i};

static _Complex long double make(int x) {
  _Complex long double r;
  __real r = x + 1;
  __imag r = x - 1;
  return r;
}

int main(void) {
  long double a = 2.5L, b = -7.0L;
  long double im = __imag make(5);
  long double re = __real make(5);
  printf("%u %u %d %d\n", __real gs.x, __imag gs.x, __real gs.y, __imag gs.y);
  printf("%.2Lf %.2Lf\n", re, im);
  printf("%.2Lf %.2Lf %.4Lf\n", __builtin_fmaxl(a, b), __builtin_fminl(a, b),
         __builtin_sqrtl(a));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
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
// CHECK-NEXT:     fn __slate_fmaxl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fminl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80_f80_f80(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     x: num_complex::Complex<u16>,
// CHECK-NEXT:     y: num_complex::Complex<i8>,
// CHECK-NEXT: }
// CHECK-NEXT: static mut gs: S = S {
// CHECK-NEXT:     x: {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = 100 as i32;
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                 re: 0 as i32,
// CHECK-NEXT:                 im: 200 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]] + [[T_2]].re,
// CHECK-NEXT:                 im: [[T_2]].im,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re as u16,
// CHECK-NEXT:             im: [[T_0]].im as u16,
// CHECK-NEXT:         }
// CHECK-NEXT:     },
// CHECK-NEXT:     y: {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = 3 as i32;
// CHECK-NEXT:             let [[T_5:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                 re: 0 as i32,
// CHECK-NEXT:                 im: 4 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_4]] - [[T_5]].re,
// CHECK-NEXT:                 im: -[[T_5]].im,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_3]].re as i8,
// CHECK-NEXT:             im: [[T_3]].im as i8,
// CHECK-NEXT:         }
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-NEXT: fn make(x: i32) -> num_complex::Complex<LongDouble> {
// CHECK-NEXT:     let mut r: num_complex::Complex<LongDouble> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r.re = __slate_f80_from_i32(x + 1);
// CHECK-NEXT:     r.im = __slate_f80_from_i32(x - 1);
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// CHECK-NEXT:     let b: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 1, 192]);
// CHECK-NEXT:     let im: LongDouble = make(5 as i32).im;
// CHECK-NEXT:     let re: LongDouble = make(5 as i32).re;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %d %d\n".as_ptr(),
// CHECK-NEXT:             ((unsafe { gs.x.re }) as u32) as i32,
// CHECK-NEXT:             ((unsafe { gs.x.im }) as u32) as i32,
// CHECK-NEXT:             (unsafe { gs.y.re }) as i32,
// CHECK-NEXT:             (unsafe { gs.y.im }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { __slate_printf__ri32_pi8_f80_f80(c"%.2Lf %.2Lf\n".as_ptr(), re, im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80_f80_f80(
// CHECK-NEXT:             c"%.2Lf %.2Lf %.4Lf\n".as_ptr(),
// CHECK-NEXT:             unsafe { __slate_fmaxl__rf80_f80_f80(a, b) },
// CHECK-NEXT:             unsafe { __slate_fminl__rf80_f80_f80(a, b) },
// CHECK-NEXT:             unsafe { __slate_sqrtl__rf80_f80(a) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
