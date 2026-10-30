/* { dg-options "-fcx-fortran-rules" } */
#include <stdio.h>

static void show(const char *name, double _Complex z) { printf("%s %a %a\n", name, __real__ z, __imag__ z); }

int main(void) {
#ifdef __STDC_IEC_559_COMPLEX__
  puts("iec_559_complex");
#endif
  volatile double big = 1e300, one = 1.0, zero = 0.0, inf = __builtin_inf();
  double _Complex z = big + big * 1.0i;
  show("big", z / z);
  show("small", (one + 2.0i) / (3.0 + 4.0i));
  show("tilted", (one + 2.0i) / (4.0 + 3.0i));
  show("equal", (one + 2.0i) / (3.0 - 3.0i));
  show("zero", (one + one * 1.0i) / (zero + zero * 1.0i));
  show("inf", (inf + one * 1.0i) / (one + one * 1.0i));
  show("mul", (inf + inf * 1.0i) * (one + zero * 1.0i));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fcx-fortran-rules" }
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_add(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_sub(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_mul(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_mul_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_cf80_neg(
// I686-UNKNOWN-LINUX-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// I686-UNKNOWN-LINUX-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// I686-UNKNOWN-LINUX-GNU-NEXT: #[derive(Clone, Copy)]
// I686-UNKNOWN-LINUX-GNU-NEXT: struct LongDouble([u8; 10]);
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Add for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_add(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Sub for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_sub(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Mul for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_mul(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Div for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_div(self, __o)
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// I686-UNKNOWN-LINUX-GNU-NEXT:         {
// I686-UNKNOWN-LINUX-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::ops::Neg for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     type Output = LongDouble;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn neg(self) -> LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_neg(self)
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// I686-UNKNOWN-LINUX-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// I686-UNKNOWN-LINUX-GNU-NEXT: }
// I686-UNKNOWN-LINUX-GNU-EMPTY:
// I686-UNKNOWN-LINUX-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// I686-UNKNOWN-LINUX-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// I686-UNKNOWN-LINUX-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// I686-UNKNOWN-LINUX-GNU-NEXT:         } else {
// I686-UNKNOWN-LINUX-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// I686-UNKNOWN-LINUX-GNU-NEXT:             } else {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// I686-UNKNOWN-LINUX-GNU-NEXT:                 } else {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     None
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             }
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const i8, z: num_complex::Complex<f64>) {
// I686-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const i8, z: num_complex::Complex<f64>) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const u8, z: num_complex::Complex<f64>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %a %a\n".as_ptr(), name, z.re, z.im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let big: f64 = 1e300f64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     let big: f64 = __slate_f80_to_f64(LongDouble([45, 221, 172, 3, 64, 228, 33, 191, 227, 67]));
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let big: f64 = 1e300f64;
// CHECK-NEXT:     let one: f64 = 1.0f64;
// CHECK-NEXT:     let zero: f64 = 0.0f64;
// CHECK-NEXT:     let inf: f64 = f64::INFINITY;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let mut z: num_complex::Complex<f64> =
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(c"big".as_ptr(), z.c_div_improved(z));
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         c"small".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             3.0f64
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 4.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     );
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         c"tilted".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             4.0f64
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 3.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     );
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         c"equal".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             3.0f64
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 - num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 3.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     );
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         c"zero".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:     let z: num_complex::Complex<f64> = {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_0:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:             let [[T_1:__t[0-9]+]] =
// I686-UNKNOWN-LINUX-GNU-NEXT:                 __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) });
// I686-UNKNOWN-LINUX-GNU-NEXT:             let [[T_2:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_3:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(big))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_4:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_5:__t[0-9]+]] = num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let mut z: num_complex::Complex<f64> =
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(c"big".as_ptr(), z.c_div_improved(z));
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         c"small".as_ptr(),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             3.0f64
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 4.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         c"tilted".as_ptr(),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             4.0f64
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 3.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         c"equal".as_ptr(),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 im: 2.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             3.0f64
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 - num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 3.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         c"zero".as_ptr(),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     );
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         c"inf".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_5]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_5]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_3]] * [[T_4]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_3]] * [[T_4]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             };
// I686-UNKNOWN-LINUX-GNU-NEXT:             num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 re: [[T_1]] + [[T_2]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 im: [[T_2]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:             }
// I686-UNKNOWN-LINUX-GNU-NEXT:         };
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_0]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_0]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     };
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"big".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_6:__t[0-9]+]] = __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_7:__t[0-9]+]] = z;
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_7]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_7]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_8:__t[0-9]+]] = z;
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: __slate_f80_from_f64([[T_8]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: __slate_f80_from_f64([[T_8]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_6]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_6]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"small".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_9:__t[0-9]+]] = __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_10:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_11:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_12:__t[0-9]+]] = num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         c"inf".as_ptr(),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_div_improved(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     );
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     show(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         c"mul".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         .c_mul(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_12]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_12]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_10]] + [[T_11]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_11]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_13:__t[0-9]+]] = __slate_f80_from_f64(3.0f64);
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_14:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_15:__t[0-9]+]] = num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     show(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         c"mul".as_ptr(),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 * num_complex::Complex {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     re: 0.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         .c_mul(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                         im: 1.0f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:                     },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             true,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         ),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     );
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 4.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_15]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_15]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_13]] + [[T_14]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_14]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_9]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_9]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"tilted".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_16:__t[0-9]+]] = __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_17:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_18:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_19:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_19]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_19]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_17]] + [[T_18]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_18]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_20:__t[0-9]+]] = __slate_f80_from_f64(4.0f64);
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_21:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_22:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 3.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_22]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_22]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_20]] + [[T_21]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_21]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_16]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_16]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"equal".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_23:__t[0-9]+]] = __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_24:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_25:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_26:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 2.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_26]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_26]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_24]] + [[T_25]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_25]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_27:__t[0-9]+]] = __slate_f80_from_f64(3.0f64);
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_28:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_29:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: 3.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: __slate_f80_from_f64([[T_29]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: __slate_f80_from_f64([[T_29]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_27]] - [[T_28]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: -[[T_28]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_23]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_23]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"zero".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_30:__t[0-9]+]] = __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_31:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_32:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_33:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     });
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_34:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         let [[T_35:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: __slate_f80_from_f64([[T_35]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: __slate_f80_from_f64([[T_35]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_33]] * [[T_34]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_33]] * [[T_34]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_31]] + [[T_32]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_32]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_36:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(zero))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_37:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_38:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         std::ptr::read_volatile(std::ptr::addr_of!(zero))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     });
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_39:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         let [[T_40:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: __slate_f80_from_f64([[T_40]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: __slate_f80_from_f64([[T_40]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_38]] * [[T_39]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_38]] * [[T_39]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_36]] + [[T_37]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_37]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_30]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_30]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"inf".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_41:__t[0-9]+]] = __slate_cf80_div_improved(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_42:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(inf))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_43:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_44:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     });
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_45:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         let [[T_46:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: __slate_f80_from_f64([[T_46]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: __slate_f80_from_f64([[T_46]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_44]] * [[T_45]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_44]] * [[T_45]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_42]] + [[T_43]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_43]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_47:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_48:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_49:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     });
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_50:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         let [[T_51:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: __slate_f80_from_f64([[T_51]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: __slate_f80_from_f64([[T_51]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_49]] * [[T_50]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_49]] * [[T_50]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_47]] + [[T_48]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_48]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_41]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_41]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// I686-UNKNOWN-LINUX-GNU-NEXT:     show(c"mul".as_ptr(), {
// I686-UNKNOWN-LINUX-GNU-NEXT:         let [[T_52:__t[0-9]+]] = __slate_cf80_mul_basic(
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_53:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(inf))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_54:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_55:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         std::ptr::read_volatile(std::ptr::addr_of!(inf))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     });
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_56:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         let [[T_57:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: __slate_f80_from_f64([[T_57]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: __slate_f80_from_f64([[T_57]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_55]] * [[T_56]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_55]] * [[T_56]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_53]] + [[T_54]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_54]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:             {
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_58:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(one))
// I686-UNKNOWN-LINUX-GNU-NEXT:                 });
// I686-UNKNOWN-LINUX-GNU-NEXT:                 let [[T_59:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_60:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         std::ptr::read_volatile(std::ptr::addr_of!(zero))
// I686-UNKNOWN-LINUX-GNU-NEXT:                     });
// I686-UNKNOWN-LINUX-GNU-NEXT:                     let [[T_61:__t[0-9]+]] = {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         let [[T_62:__t[0-9]+]] = num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: 0.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: 1.0f64,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         };
// I686-UNKNOWN-LINUX-GNU-NEXT:                         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                             re: __slate_f80_from_f64([[T_62]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:                             im: __slate_f80_from_f64([[T_62]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:                         }
// I686-UNKNOWN-LINUX-GNU-NEXT:                     };
// I686-UNKNOWN-LINUX-GNU-NEXT:                     num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                         re: [[T_60]] * [[T_61]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                         im: [[T_60]] * [[T_61]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     }
// I686-UNKNOWN-LINUX-GNU-NEXT:                 };
// I686-UNKNOWN-LINUX-GNU-NEXT:                 num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:                     re: [[T_58]] + [[T_59]].re,
// I686-UNKNOWN-LINUX-GNU-NEXT:                     im: [[T_59]].im,
// I686-UNKNOWN-LINUX-GNU-NEXT:                 }
// I686-UNKNOWN-LINUX-GNU-NEXT:             },
// I686-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         num_complex::Complex {
// I686-UNKNOWN-LINUX-GNU-NEXT:             re: __slate_f80_to_f64([[T_52]].re),
// I686-UNKNOWN-LINUX-GNU-NEXT:             im: __slate_f80_to_f64([[T_52]].im),
// I686-UNKNOWN-LINUX-GNU-NEXT:         }
// I686-UNKNOWN-LINUX-GNU-NEXT:     });
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                         im: 1.0f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:                     },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             true,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         ),
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
