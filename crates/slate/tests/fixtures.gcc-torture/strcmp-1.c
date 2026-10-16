/* Copyright (C) 2002  Free Software Foundation.

   Test strcmp with various combinations of pointer alignments and lengths to
   make sure any optimizations in the library are correct.

   Written by Michael Meissner, March 9, 2002.  */

#include <stddef.h>
#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_TEST
#define MAX_TEST (8 * sizeof(long long))
#endif

#ifndef MAX_EXTRA
#define MAX_EXTRA (sizeof(long long))
#endif

#define MAX_LENGTH (MAX_OFFSET + MAX_TEST + MAX_EXTRA + 2)

static union {
  unsigned char buf[MAX_LENGTH];
  long long     align_int;
  long double   align_fp;
} u1, u2;

void test(const unsigned char *s1, const unsigned char *s2, int expected) {
  int value = strcmp((char *)s1, (char *)s2);

  if (expected < 0 && value >= 0)
    abort();
  else if (expected == 0 && value != 0)
    abort();
  else if (expected > 0 && value <= 0)
    abort();
}

int main(void) {
  size_t         off1, off2, len, i;
  unsigned char *buf1, *buf2;
  unsigned char *mod1, *mod2;
  unsigned char *p1, *p2;

  for (off1 = 0; off1 < MAX_OFFSET; off1++)
    for (off2 = 0; off2 < MAX_OFFSET; off2++)
      for (len = 0; len < MAX_TEST; len++) {
        p1 = u1.buf;
        for (i = 0; i < off1; i++)
          *p1++ = '\0';

        buf1 = p1;
        for (i = 0; i < len; i++)
          *p1++ = 'a';

        mod1 = p1;
        for (i = 0; i < MAX_EXTRA + 2; i++)
          *p1++ = 'x';

        p2 = u2.buf;
        for (i = 0; i < off2; i++)
          *p2++ = '\0';

        buf2 = p2;
        for (i = 0; i < len; i++)
          *p2++ = 'a';

        mod2 = p2;
        for (i = 0; i < MAX_EXTRA + 2; i++)
          *p2++ = 'x';

        mod1[0] = '\0';
        mod2[0] = '\0';
        test(buf1, buf2, 0);

        mod1[0] = 'a';
        mod1[1] = '\0';
        mod2[0] = '\0';
        test(buf1, buf2, +1);

        mod1[0] = '\0';
        mod2[0] = 'a';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = 'b';
        mod1[1] = '\0';
        mod2[0] = 'c';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = 'c';
        mod1[1] = '\0';
        mod2[0] = 'b';
        mod2[1] = '\0';
        test(buf1, buf2, +1);

        mod1[0] = 'b';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\251';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = (unsigned char)'\251';
        mod1[1] = '\0';
        mod2[0] = 'b';
        mod2[1] = '\0';
        test(buf1, buf2, +1);

        mod1[0] = (unsigned char)'\251';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\252';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = (unsigned char)'\252';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\251';
        mod2[1] = '\0';
        test(buf1, buf2, +1);
      }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test strcmp with various combinations of pointer alignments and lengths to
// CHECK-NEXT: //! make sure any optimizations in the library are correct.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Michael Meissner, March 9, 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
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
// CHECK-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
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
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     buf: [u8; 82],
// CHECK-NEXT:     align_int: i64,
// CHECK-NEXT:     align_fp: LongDouble,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u1: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut u2: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut s1: *const u8, mut s2: *const u8, mut expected: i32) {
// CHECK-NEXT:     let mut value: i32 = unsafe { strcmp(s1 as *const i8, s2 as *const i8) };
// CHECK-NEXT:     if expected < (0 as i32) && value >= (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if expected == (0 as i32) && value != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if expected > (0 as i32) && value <= (0 as i32) {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut off1: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     let mut off2: u64 = 0 as u64;
// CHECK-NEXT:     let mut len: u64 = 0 as u64;
// CHECK-NEXT:     let mut i: u64 = 0 as u64;
// CHECK-NEXT:     let mut buf1: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut buf2: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut mod1: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut mod2: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p1: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p2: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while off1 < (8 as u64) {
// CHECK-NEXT:         off2 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while off2 < (8 as u64) {
// CHECK-NEXT:             len = ((0 as i32) as i64) as u64;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while len < (((8 as i32) as i64) as u64).wrapping_mul(8 as u64) {
// CHECK-NEXT:                 p1 = unsafe { std::ptr::addr_of_mut!(u1.buf) as *mut u8 };
// CHECK-NEXT:                 i = ((0 as i32) as i64) as u64;
// CHECK-NEXT:                 '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < off1 {
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: *mut u8 = p1;
// CHECK-NEXT:                     let mut [[V_2:__v[0-9]+]]: *mut u8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     p1 = [[V_2]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *[[V_1]] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_3:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                     let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                     i = [[V_4]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 buf1 = p1;
// CHECK-NEXT:                 i = ((0 as i32) as i64) as u64;
// CHECK-NEXT:                 '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:                     let mut [[V_5:__v[0-9]+]]: *mut u8 = p1;
// CHECK-NEXT:                     let mut [[V_6:__v[0-9]+]]: *mut u8 = unsafe { [[V_5]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     p1 = [[V_6]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *[[V_5]] = ((97 as i32) as i8) as u8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_7:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                     let mut [[V_8:__v[0-9]+]]: u64 = [[V_7]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                     i = [[V_8]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 mod1 = p1;
// CHECK-NEXT:                 i = ((0 as i32) as i64) as u64;
// CHECK-NEXT:                 '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < (8 as u64).wrapping_add(((2 as i32) as i64) as u64) {
// CHECK-NEXT:                     let mut [[V_9:__v[0-9]+]]: *mut u8 = p1;
// CHECK-NEXT:                     let mut [[V_10:__v[0-9]+]]: *mut u8 = unsafe { [[V_9]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     p1 = [[V_10]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *[[V_9]] = ((120 as i32) as i8) as u8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_11:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                     let mut [[V_12:__v[0-9]+]]: u64 = [[V_11]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                     i = [[V_12]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 p2 = unsafe { std::ptr::addr_of_mut!(u2.buf) as *mut u8 };
// CHECK-NEXT:                 i = ((0 as i32) as i64) as u64;
// CHECK-NEXT:                 '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while i < off2 {
// CHECK-NEXT:                     let mut [[V_13:__v[0-9]+]]: *mut u8 = p2;
// CHECK-NEXT:                     let mut [[V_14:__v[0-9]+]]: *mut u8 = unsafe { [[V_13]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     p2 = [[V_14]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *[[V_13]] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_15:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                     let mut [[V_16:__v[0-9]+]]: u64 = [[V_15]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                     i = [[V_16]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 buf2 = p2;
// CHECK-NEXT:                 i = ((0 as i32) as i64) as u64;
// CHECK-NEXT:                 '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:                     let mut [[V_17:__v[0-9]+]]: *mut u8 = p2;
// CHECK-NEXT:                     let mut [[V_18:__v[0-9]+]]: *mut u8 = unsafe { [[V_17]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     p2 = [[V_18]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *[[V_17]] = ((97 as i32) as i8) as u8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_19:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                     let mut [[V_20:__v[0-9]+]]: u64 = [[V_19]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                     i = [[V_20]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 mod2 = p2;
// CHECK-NEXT:                 i = ((0 as i32) as i64) as u64;
// CHECK-NEXT:                 '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: while i < (8 as u64).wrapping_add(((2 as i32) as i64) as u64) {
// CHECK-NEXT:                     let mut [[V_21:__v[0-9]+]]: *mut u8 = p2;
// CHECK-NEXT:                     let mut [[V_22:__v[0-9]+]]: *mut u8 = unsafe { [[V_21]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     p2 = [[V_22]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *[[V_21]] = ((120 as i32) as i8) as u8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_23:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:                     let mut [[V_24:__v[0-9]+]]: u64 = [[V_23]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                     i = [[V_24]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, 0 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((97 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, 1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((97 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, -(1 as i32));
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((98 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((99 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, -(1 as i32));
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((99 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((98 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, 1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((98 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((-87 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, -(1 as i32));
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((-87 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((98 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, 1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((-87 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((-86 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, -(1 as i32));
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((0 as i32) as isize) } = ((-86 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod1.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((0 as i32) as isize) } = ((-87 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { mod2.offset((1 as i32) as isize) } = ((0 as i32) as i8) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 test(buf1 as *const u8, buf2 as *const u8, 1 as i32);
// CHECK-NEXT:                 let mut [[V_25:__v[0-9]+]]: u64 = len;
// CHECK-NEXT:                 let mut [[V_26:__v[0-9]+]]: u64 = [[V_25]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:                 len = [[V_26]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_27:__v[0-9]+]]: u64 = off2;
// CHECK-NEXT:             let mut [[V_28:__v[0-9]+]]: u64 = [[V_27]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:             off2 = [[V_28]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: u64 = off1;
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: u64 = [[V_29]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         off1 = [[V_30]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
