/* Copyright (C) 2002  Free Software Foundation.

   Test memset with various combinations of pointer alignments and constant
   lengths to make sure any optimizations in the compiler are correct.

   Written by Roger Sayle, April 22, 2002.  */

#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_COPY
#define MAX_COPY 15
#endif

#ifndef MAX_EXTRA
#define MAX_EXTRA (sizeof(long long))
#endif

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u;

char A = 'A';

void reset() {
  int i;

  for (i = 0; i < MAX_LENGTH; i++)
    u.buf[i] = 'a';
}

void check(int off, int len, int ch) {
  char *q;
  int   i;

  q = u.buf;
  for (i = 0; i < off; i++, q++)
    if (*q != 'a')
      abort();

  for (i = 0; i < len; i++, q++)
    if (*q != ch)
      abort();

  for (i = 0; i < MAX_EXTRA; i++, q++)
    if (*q != 'a')
      abort();
}

int main() {
  int   off;
  char *p;

  /* len == 1 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 1);
    if (p != u.buf + off)
      abort();
    check(off, 1, '\0');

    p = memset(u.buf + off, A, 1);
    if (p != u.buf + off)
      abort();
    check(off, 1, 'A');

    p = memset(u.buf + off, 'B', 1);
    if (p != u.buf + off)
      abort();
    check(off, 1, 'B');
  }

  /* len == 2 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 2);
    if (p != u.buf + off)
      abort();
    check(off, 2, '\0');

    p = memset(u.buf + off, A, 2);
    if (p != u.buf + off)
      abort();
    check(off, 2, 'A');

    p = memset(u.buf + off, 'B', 2);
    if (p != u.buf + off)
      abort();
    check(off, 2, 'B');
  }

  /* len == 3 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 3);
    if (p != u.buf + off)
      abort();
    check(off, 3, '\0');

    p = memset(u.buf + off, A, 3);
    if (p != u.buf + off)
      abort();
    check(off, 3, 'A');

    p = memset(u.buf + off, 'B', 3);
    if (p != u.buf + off)
      abort();
    check(off, 3, 'B');
  }

  /* len == 4 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 4);
    if (p != u.buf + off)
      abort();
    check(off, 4, '\0');

    p = memset(u.buf + off, A, 4);
    if (p != u.buf + off)
      abort();
    check(off, 4, 'A');

    p = memset(u.buf + off, 'B', 4);
    if (p != u.buf + off)
      abort();
    check(off, 4, 'B');
  }

  /* len == 5 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 5);
    if (p != u.buf + off)
      abort();
    check(off, 5, '\0');

    p = memset(u.buf + off, A, 5);
    if (p != u.buf + off)
      abort();
    check(off, 5, 'A');

    p = memset(u.buf + off, 'B', 5);
    if (p != u.buf + off)
      abort();
    check(off, 5, 'B');
  }

  /* len == 6 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 6);
    if (p != u.buf + off)
      abort();
    check(off, 6, '\0');

    p = memset(u.buf + off, A, 6);
    if (p != u.buf + off)
      abort();
    check(off, 6, 'A');

    p = memset(u.buf + off, 'B', 6);
    if (p != u.buf + off)
      abort();
    check(off, 6, 'B');
  }

  /* len == 7 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 7);
    if (p != u.buf + off)
      abort();
    check(off, 7, '\0');

    p = memset(u.buf + off, A, 7);
    if (p != u.buf + off)
      abort();
    check(off, 7, 'A');

    p = memset(u.buf + off, 'B', 7);
    if (p != u.buf + off)
      abort();
    check(off, 7, 'B');
  }

  /* len == 8 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 8);
    if (p != u.buf + off)
      abort();
    check(off, 8, '\0');

    p = memset(u.buf + off, A, 8);
    if (p != u.buf + off)
      abort();
    check(off, 8, 'A');

    p = memset(u.buf + off, 'B', 8);
    if (p != u.buf + off)
      abort();
    check(off, 8, 'B');
  }

  /* len == 9 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 9);
    if (p != u.buf + off)
      abort();
    check(off, 9, '\0');

    p = memset(u.buf + off, A, 9);
    if (p != u.buf + off)
      abort();
    check(off, 9, 'A');

    p = memset(u.buf + off, 'B', 9);
    if (p != u.buf + off)
      abort();
    check(off, 9, 'B');
  }

  /* len == 10 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 10);
    if (p != u.buf + off)
      abort();
    check(off, 10, '\0');

    p = memset(u.buf + off, A, 10);
    if (p != u.buf + off)
      abort();
    check(off, 10, 'A');

    p = memset(u.buf + off, 'B', 10);
    if (p != u.buf + off)
      abort();
    check(off, 10, 'B');
  }

  /* len == 11 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 11);
    if (p != u.buf + off)
      abort();
    check(off, 11, '\0');

    p = memset(u.buf + off, A, 11);
    if (p != u.buf + off)
      abort();
    check(off, 11, 'A');

    p = memset(u.buf + off, 'B', 11);
    if (p != u.buf + off)
      abort();
    check(off, 11, 'B');
  }

  /* len == 12 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 12);
    if (p != u.buf + off)
      abort();
    check(off, 12, '\0');

    p = memset(u.buf + off, A, 12);
    if (p != u.buf + off)
      abort();
    check(off, 12, 'A');

    p = memset(u.buf + off, 'B', 12);
    if (p != u.buf + off)
      abort();
    check(off, 12, 'B');
  }

  /* len == 13 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 13);
    if (p != u.buf + off)
      abort();
    check(off, 13, '\0');

    p = memset(u.buf + off, A, 13);
    if (p != u.buf + off)
      abort();
    check(off, 13, 'A');

    p = memset(u.buf + off, 'B', 13);
    if (p != u.buf + off)
      abort();
    check(off, 13, 'B');
  }

  /* len == 14 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 14);
    if (p != u.buf + off)
      abort();
    check(off, 14, '\0');

    p = memset(u.buf + off, A, 14);
    if (p != u.buf + off)
      abort();
    check(off, 14, 'A');

    p = memset(u.buf + off, 'B', 14);
    if (p != u.buf + off)
      abort();
    check(off, 14, 'B');
  }

  /* len == 15 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 15);
    if (p != u.buf + off)
      abort();
    check(off, 15, '\0');

    p = memset(u.buf + off, A, 15);
    if (p != u.buf + off)
      abort();
    check(off, 15, 'A');

    p = memset(u.buf + off, 'B', 15);
    if (p != u.buf + off)
      abort();
    check(off, 15, 'B');
  }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test memset with various combinations of pointer alignments and constant
// CHECK-NEXT: //! lengths to make sure any optimizations in the compiler are correct.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, April 22, 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
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
// CHECK-NEXT:     buf: [i8; 31],
// CHECK-NEXT:     align_int: i64,
// CHECK-NEXT:     align_fp: LongDouble,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut A: i8 = (65 as i32) as i8;
// CHECK-EMPTY:
// CHECK-NEXT: fn reset() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64)
// CHECK-NEXT:         < (8 as u64)
// CHECK-NEXT:             .wrapping_add(((15 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_add(8 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(i as isize) } =
// CHECK-NEXT:                 (97 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check(mut off: i32, mut len: i32, mut ch: i32) {
// CHECK-NEXT:     let mut q: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     q = unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 };
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < off {
// CHECK-NEXT:         if ((unsafe { *q }) as i32) != (97 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i8 = unsafe { [[V_5]].offset((1 as i32) as isize) };
// CHECK-NEXT:         q = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         if ((unsafe { *q }) as i32) != ch {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: *mut i8 = unsafe { [[V_9]].offset((1 as i32) as isize) };
// CHECK-NEXT:         q = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         if ((unsafe { *q }) as i32) != (97 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] + (1 as i32);
// CHECK-NEXT:         i = [[V_12]];
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: *mut i8 = unsafe { [[V_13]].offset((1 as i32) as isize) };
// CHECK-NEXT:         q = [[V_14]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // len == 1
// CHECK-NEXT:     let mut off: i32 = 0 as i32;
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((1 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 1 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((1 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 1 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((1 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 1 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i32 = [[V_15]] + (1 as i32);
// CHECK-NEXT:         off = [[V_16]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 2
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((2 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 2 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((2 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 2 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((2 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 2 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: i32 = [[V_17]] + (1 as i32);
// CHECK-NEXT:         off = [[V_18]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 3
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((3 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 3 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((3 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 3 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((3 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 3 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i32 = [[V_19]] + (1 as i32);
// CHECK-NEXT:         off = [[V_20]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 4
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((4 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 4 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((4 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 4 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((4 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 4 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: i32 = [[V_21]] + (1 as i32);
// CHECK-NEXT:         off = [[V_22]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 5
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((5 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 5 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((5 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 5 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((5 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 5 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: i32 = [[V_23]] + (1 as i32);
// CHECK-NEXT:         off = [[V_24]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 6
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((6 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 6 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((6 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 6 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((6 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 6 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: i32 = [[V_25]] + (1 as i32);
// CHECK-NEXT:         off = [[V_26]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 7
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((7 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 7 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((7 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 7 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((7 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 7 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: i32 = [[V_27]] + (1 as i32);
// CHECK-NEXT:         off = [[V_28]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 8
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((8 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 8 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((8 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 8 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((8 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 8 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: i32 = [[V_29]] + (1 as i32);
// CHECK-NEXT:         off = [[V_30]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 9
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((9 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 9 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((9 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 9 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((9 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 9 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_31:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_32:__v[0-9]+]]: i32 = [[V_31]] + (1 as i32);
// CHECK-NEXT:         off = [[V_32]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 10
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((10 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 10 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((10 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 10 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((10 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 10 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_33:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_34:__v[0-9]+]]: i32 = [[V_33]] + (1 as i32);
// CHECK-NEXT:         off = [[V_34]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 11
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((11 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 11 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((11 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 11 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((11 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 11 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_35:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_36:__v[0-9]+]]: i32 = [[V_35]] + (1 as i32);
// CHECK-NEXT:         off = [[V_36]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 12
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((12 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 12 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((12 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 12 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((12 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 12 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_37:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_38:__v[0-9]+]]: i32 = [[V_37]] + (1 as i32);
// CHECK-NEXT:         off = [[V_38]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 13
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((13 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 13 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((13 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 13 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((13 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 13 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_39:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_40:__v[0-9]+]]: i32 = [[V_39]] + (1 as i32);
// CHECK-NEXT:         off = [[V_40]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 14
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((14 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 14 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((14 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 14 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((14 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 14 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_41:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_42:__v[0-9]+]]: i32 = [[V_41]] + (1 as i32);
// CHECK-NEXT:         off = [[V_42]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // len == 15
// CHECK-NEXT:     off = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: while ((off as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         reset();
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:                 ((15 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 15 as i32, 0 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 (unsafe { A }) as i32,
// CHECK-NEXT:                 ((15 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 15 as i32, 65 as i32);
// CHECK-NEXT:         p = (unsafe {
// CHECK-NEXT:             memset(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize)
// CHECK-NEXT:                 }) as *mut (),
// CHECK-NEXT:                 66 as i32,
// CHECK-NEXT:                 ((15 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:         if p != unsafe { unsafe { std::ptr::addr_of_mut!(u.buf) as *mut i8 }.offset(off as isize) }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         check(off, 15 as i32, 66 as i32);
// CHECK-NEXT:         let mut [[V_43:__v[0-9]+]]: i32 = off;
// CHECK-NEXT:         let mut [[V_44:__v[0-9]+]]: i32 = [[V_43]] + (1 as i32);
// CHECK-NEXT:         off = [[V_44]];
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
