#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int string2ld(const char *s, size_t slen, long double *dp) {
  char buf[64];
  char *eptr;
  if (slen == 0 || slen >= sizeof(buf)) return 0;
  memcpy(buf, s, slen);
  buf[slen] = '\0';
  long double value = strtold(buf, &eptr);
  if (*eptr != '\0' || isnan(value)) return 0;
  if (dp) *dp = value;
  return 1;
}

static int ld2string(char *buf, size_t len, long double value) {
  if (isinf(value)) {
    if (len < 5) return 0;
    if (value > 0) {
      memcpy(buf, "inf", 3);
      return 3;
    }
    memcpy(buf, "-inf", 4);
    return 4;
  }
  return snprintf(buf, len, "%.17Lg", value);
}

static int arOpAccumulate(long double *acc, long double v) {
  long double next = *acc + v;
  if (isnan(next) || isinf(next)) return 0;
  *acc = next;
  return 1;
}

static int classify(long double x) {
  return !!isnan(x) | !!isinf(x) << 1 | !!isfinite(x) << 2 |
         !!isnormal(x) << 3 | !!signbit(x) << 4 |
         (fpclassify(x) == FP_SUBNORMAL) << 5 | (fpclassify(x) == FP_ZERO) << 6;
}

int main(void) {
  long double values[] = {
      0.0L,          -0.0L,          1.5L,
      -2.25L,        LDBL_MAX,       -LDBL_MAX,
      LDBL_MIN,      LDBL_MIN / 4.0L, -LDBL_MIN / 1024.0L,
      HUGE_VALL,     -HUGE_VALL,     LDBL_MAX * 4.0L,
      nanl(""),      -nanl("7"),
  };
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    printf("%zu %#x\n", i, classify(values[i]));

  const char *inputs[] = {"3.25", "nan", "-inf", "1e5000", "12x", ""};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
    long double d = -1.0L;
    int ok = string2ld(inputs[i], strlen(inputs[i]), &d);
    char out[64];
    int n = ok ? ld2string(out, sizeof(out), d) : 0;
    printf("%d %.*s\n", ok, n, out);
  }

  long double acc = 0.0L;
  long double addends[] = {1.0L, LDBL_MAX, LDBL_MAX, 2.0L, -HUGE_VALL};
  int accepted = 0;
  for (size_t i = 0; i < sizeof(addends) / sizeof(addends[0]); i++)
    accepted += arOpAccumulate(&acc, addends[i]);
  char out[64];
  int n = ld2string(out, sizeof(out), acc);
  printf("%d %.*s\n", accepted, n, out);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn snprintf([[ARG_0]]: *mut i8, [[ARG_1:arg[0-9]+]]: u64, [[ARG_2:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcpy([[ARG_0]]: *mut (), [[ARG_1]]: *const (), [[ARG_2]]: u64) -> *mut ();
// CHECK-NEXT:     fn strlen([[ARG_0]]: *const i8) -> u64;
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
// CHECK-NEXT:     fn __slate_snprintf__ri32_pi8_u64_pi8_f80(
// CHECK-NEXT:         _0: *mut i8,
// CHECK-NEXT:         _1: u64,
// CHECK-NEXT:         _2: *const i8,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn __slate_strtold__rf80_pi8_ppi8(_0: *const i8, _1: *mut *mut i8) -> LongDouble;
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn string2ld(s: *const i8, slen: u64, dp: *mut LongDouble) -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     let mut eptr: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if slen == 0 || slen >= 64 {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             s as *const u8,
// CHECK-NEXT:             buf.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             slen as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     buf.0[slen as usize] = 0;
// CHECK-NEXT:     let value: LongDouble = unsafe {
// CHECK-NEXT:         __slate_strtold__rf80_pi8_ppi8(
// CHECK-NEXT:             buf.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:             std::ptr::addr_of_mut!(eptr),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe { *eptr }) as i32) != 0 || __slate_f80_is_fp_class(value, 3) {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !dp.is_null() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *dp = value;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn ld2string(buf: *mut i8, len: u64, value: LongDouble) -> i32 {
// CHECK-NEXT:     if (if __slate_f80_is_fp_class(value, 516) {
// CHECK-NEXT:         if __slate_f80_signbit(value) { -1i32 } else { 1i32 }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0i32
// CHECK-NEXT:     }) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         if len < 5 {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         if value > __slate_f80_from_i32(0i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::ptr::copy_nonoverlapping(
// CHECK-NEXT:                     c"inf".as_ptr() as *const u8,
// CHECK-NEXT:                     buf as *mut u8,
// CHECK-NEXT:                     3 as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             return 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::copy_nonoverlapping(
// CHECK-NEXT:                 c"-inf".as_ptr() as *const u8,
// CHECK-NEXT:                 buf as *mut u8,
// CHECK-NEXT:                 4 as usize,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_snprintf__ri32_pi8_u64_pi8_f80(buf, len, c"%.17Lg".as_ptr(), value)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn arOpAccumulate(acc: &mut LongDouble, v: LongDouble) -> i32 {
// CHECK-NEXT:     let next: LongDouble = (unsafe { *(acc as *mut LongDouble) }) + v;
// CHECK-NEXT:     if __slate_f80_is_fp_class(next, 3)
// CHECK-NEXT:         || (if __slate_f80_is_fp_class(next, 516) {
// CHECK-NEXT:             if __slate_f80_signbit(next) { -1i32 } else { 1i32 }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(acc as *mut LongDouble) = next;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(x: LongDouble) -> i32 {
// CHECK-NEXT:     (!(!__slate_f80_is_fp_class(x, 3)) as i32)
// CHECK-NEXT:         | (((if __slate_f80_is_fp_class(x, 516) {
// CHECK-NEXT:             if __slate_f80_signbit(x) { -1i32 } else { 1i32 }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }) != 0) as i32) << 1i32 | (!(!__slate_f80_is_fp_class(x, 504)) as i32) << 2i32
// CHECK-NEXT:         | (!(!__slate_f80_is_fp_class(x, 264)) as i32) << 3i32
// CHECK-NEXT:         | (!(!__slate_f80_signbit(x)) as i32) << 4i32
// CHECK-NEXT:         | (((if __slate_f80_is_fp_class(x, 3) {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_is_fp_class(x, 516) {
// CHECK-NEXT:                 1i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_is_fp_class(x, 264) {
// CHECK-NEXT:                     4i32
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if __slate_f80_is_fp_class(x, 144) { 3i32 } else { 2i32 }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }) == 3) as i32) << 5i32
// CHECK-NEXT:         | (((if __slate_f80_is_fp_class(x, 3) {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_is_fp_class(x, 516) {
// CHECK-NEXT:                 1i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_is_fp_class(x, 264) {
// CHECK-NEXT:                     4i32
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if __slate_f80_is_fp_class(x, 144) { 3i32 } else { 2i32 }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }) == 2) as i32) << 6i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: [LongDouble; 14] = [
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 0, 192]),
// CHECK-NEXT:         LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]),
// CHECK-NEXT:         LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 255]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 32, 0, 0]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 32, 0, 0, 128]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 255]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// CHECK-NEXT:         LongDouble([7, 0, 0, 0, 0, 0, 0, 192, 255, 255]),
// CHECK-NEXT:     ];
// CHECK-NEXT:     for i in 0..14u64 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%zu %#x\n".as_ptr(), i, classify(values[i as usize])) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let inputs: __SlateAlign16<[*const i8; 6]> = __SlateAlign16([
// CHECK-NEXT:         c"3.25".as_ptr(),
// CHECK-NEXT:         c"nan".as_ptr(),
// CHECK-NEXT:         c"-inf".as_ptr(),
// CHECK-NEXT:         c"1e5000".as_ptr(),
// CHECK-NEXT:         c"12x".as_ptr(),
// CHECK-NEXT:         c"".as_ptr(),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0..6u64 {
// CHECK-NEXT:         let mut d: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// CHECK-NEXT:         let ok: i32 = string2ld(
// CHECK-NEXT:             inputs.0[i as usize],
// CHECK-NEXT:             unsafe { strlen(inputs.0[i as usize]) },
// CHECK-NEXT:             std::ptr::addr_of_mut!(d),
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut out: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:         let mut n: i32 = 0;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = if ok != 0 {
// CHECK-NEXT:             ld2string(out.0.as_mut_ptr() as *mut i8, 64u64, d)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0
// CHECK-NEXT:         };
// CHECK-NEXT:         n = [[V_0]];
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d %.*s\n".as_ptr(), ok, n, out.0.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut acc: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let addends: [LongDouble; 5] = [
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:         LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]),
// CHECK-NEXT:         LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 255]),
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut accepted: i32 = 0;
// CHECK-NEXT:     for i in 0..5u64 {
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = arOpAccumulate(
// CHECK-NEXT:             unsafe { &mut (*std::ptr::addr_of_mut!(acc)) },
// CHECK-NEXT:             addends[i as usize],
// CHECK-NEXT:         );
// CHECK-NEXT:         accepted += [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut out: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     let n: i32 = ld2string(out.0.as_mut_ptr() as *mut i8, 64u64, acc);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %.*s\n".as_ptr(), accepted, n, out.0.as_mut_ptr() as *mut i8) };
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
