#include <stdarg.h>
#include <stdio.h>

static long double sum_long_doubles(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = 0.0L;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, long double);
  }
  va_end(ap);
  return total;
}

static long double mixed(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = 0.0L;
  for (int i = 0; i < count; i++) {
    int kind = va_arg(ap, int);
    if (kind == 0) {
      total += va_arg(ap, int);
    } else if (kind == 1) {
      total += va_arg(ap, double);
    } else {
      total += va_arg(ap, long double);
    }
  }
  va_end(ap);
  return total;
}

static long double first_twice(int unused, ...) {
  va_list ap, copy;
  va_start(ap, unused);
  va_copy(copy, ap);
  long double first  = va_arg(ap, long double);
  long double second = va_arg(ap, long double);
  long double again  = va_arg(copy, long double);
  va_end(copy);
  va_end(ap);
  return first * 100.0L + second * 10.0L + again;
}

int main(void) {
  long double precise = 1.0L + 0x1p-63L;
  printf("%.3Lf\n", sum_long_doubles(3, 1.25L, 2.5L, -0.75L));
  printf("%La\n", sum_long_doubles(2, precise, 0.0L));
  printf("%.3Lf\n", mixed(12, 0, 1, 1, 0.5, 2, 1.5L, 1, 2.0, 1, 3.0, 1, 4.0, 1, 5.0, 1, 6.0, 1, 7.0,
                          1, 8.0, 2, 0.25L, 1, 9.0));
  printf("%.1Lf\n", first_twice(0, 3.0L, 4.0L));
  return (int)sum_long_doubles(2, 20.5L, 21.5L);
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
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'_>) -> LongDouble;
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
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *const i8, _1: LongDouble) -> i32;
// CHECK-NEXT:     fn __slate_vcall__rf80_i32__f80_f80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_vcall__rf80_i32__f80_f80_f80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:         _4: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_vcall__rf80_i32__i32_i32_i32_f64_i32_f80_i32_f64_i32_f64_i32_f64_i32_f64_i32_f64_i32_f64_i32_f64_i32_f80_i32_f64(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: i32,
// CHECK-NEXT:         _3: i32,
// CHECK-NEXT:         _4: i32,
// CHECK-NEXT:         _5: f64,
// CHECK-NEXT:         _6: i32,
// CHECK-NEXT:         _7: LongDouble,
// CHECK-NEXT:         _8: i32,
// CHECK-NEXT:         _9: f64,
// CHECK-NEXT:         _10: i32,
// CHECK-NEXT:         _11: f64,
// CHECK-NEXT:         _12: i32,
// CHECK-NEXT:         _13: f64,
// CHECK-NEXT:         _14: i32,
// CHECK-NEXT:         _15: f64,
// CHECK-NEXT:         _16: i32,
// CHECK-NEXT:         _17: f64,
// CHECK-NEXT:         _18: i32,
// CHECK-NEXT:         _19: f64,
// CHECK-NEXT:         _20: i32,
// CHECK-NEXT:         _21: f64,
// CHECK-NEXT:         _22: i32,
// CHECK-NEXT:         _23: LongDouble,
// CHECK-NEXT:         _24: i32,
// CHECK-NEXT:         _25: f64,
// CHECK-NEXT:     ) -> LongDouble;
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
// CHECK-NEXT: unsafe extern "C-unwind" fn sum_long_doubles(
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> LongDouble {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let mut total: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: LongDouble = total;
// CHECK-NEXT:         total = [[V_0]] + unsafe { __slate_f80_va_arg(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn mixed(count: i32, mut __va_args: ...) -> LongDouble {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     let mut total: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let kind: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         if kind == 0 {
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: LongDouble = total;
// CHECK-NEXT:             total = [[V_1]] + __slate_f80_from_i32(unsafe { ap.next_arg::<i32>() });
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if kind == 1 {
// CHECK-NEXT:                 let [[V_2:__v[0-9]+]]: LongDouble = total;
// CHECK-NEXT:                 total = [[V_2]] + __slate_f80_from_f64(unsafe { ap.next_arg::<f64>() });
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let [[V_3:__v[0-9]+]]: LongDouble = total;
// CHECK-NEXT:                 total = [[V_3]]
// CHECK-NEXT:                     + unsafe { __slate_f80_va_arg(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn first_twice(unused: i32, mut __va_args: ...) -> LongDouble {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut copy: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     copy = ap.clone();
// CHECK-NEXT:     let first: LongDouble = unsafe { __slate_f80_va_arg(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT:     let second: LongDouble = unsafe { __slate_f80_va_arg(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT:     let again: LongDouble = unsafe { __slate_f80_va_arg(std::ptr::addr_of_mut!(copy)) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     {}
// CHECK-NEXT:     first * LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64])
// CHECK-NEXT:         + second * LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]) + again
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let precise: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63])
// CHECK-NEXT:         + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.3Lf\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_vcall__rf80_i32__f80_f80_f80(
// CHECK-NEXT:                     sum_long_doubles as *const (),
// CHECK-NEXT:                     3 as i32,
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 63]),
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]),
// CHECK-NEXT:                     -LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 254, 63]),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%La\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_vcall__rf80_i32__f80_f80(
// CHECK-NEXT:                     sum_long_doubles as *const (),
// CHECK-NEXT:                     2 as i32,
// CHECK-NEXT:                     precise,
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.3Lf\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_vcall__rf80_i32__i32_i32_i32_f64_i32_f80_i32_f64_i32_f64_i32_f64_i32_f64_i32_f64_i32_f64_i32_f64_i32_f80_i32_f64(
// CHECK-NEXT:                     mixed as *const (),
// CHECK-NEXT:                     12 as i32,
// CHECK-NEXT:                     0 as i32,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     0.5f64,
// CHECK-NEXT:                     2 as i32,
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]),
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     2.0f64,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     3.0f64,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     4.0f64,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     5.0f64,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     6.0f64,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     7.0f64,
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     8.0f64,
// CHECK-NEXT:                     2 as i32,
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]),
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     9.0f64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.1Lf\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_vcall__rf80_i32__f80_f80(
// CHECK-NEXT:                     first_twice as *const (),
// CHECK-NEXT:                     0 as i32,
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:                     LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     __slate_f80_to_i32(unsafe {
// CHECK-NEXT:         __slate_vcall__rf80_i32__f80_f80(
// CHECK-NEXT:             sum_long_doubles as *const (),
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 164, 3, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 172, 3, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     })
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
