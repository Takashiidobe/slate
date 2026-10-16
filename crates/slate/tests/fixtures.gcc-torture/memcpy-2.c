/* Copyright (C) 2002  Free Software Foundation.

   Test memcpy with various combinations of pointer alignments and lengths to
   make sure any optimizations in the library are correct.

   Written by Michael Meissner, March 9, 2002.  */

#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_COPY
#define MAX_COPY (10 * sizeof(long long))
#endif

#ifndef MAX_EXTRA
#define MAX_EXTRA (sizeof(long long))
#endif

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

/* Use a sequence length that is not divisible by two, to make it more
   likely to detect when words are mixed up.  */
#define SEQUENCE_LENGTH 31

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u1, u2;

int main(void) {
  int   off1, off2, len, i;
  char *p, *q, c;

  for (off1 = 0; off1 < MAX_OFFSET; off1++)
    for (off2 = 0; off2 < MAX_OFFSET; off2++)
      for (len = 1; len < MAX_COPY; len++) {
        for (i = 0, c = 'A'; i < MAX_LENGTH; i++, c++) {
          u1.buf[i] = 'a';
          if (c >= 'A' + SEQUENCE_LENGTH)
            c = 'A';
          u2.buf[i] = c;
        }

        p = memcpy(u1.buf + off1, u2.buf + off2, len);
        if (p != u1.buf + off1)
          abort();

        q = u1.buf;
        for (i = 0; i < off1; i++, q++)
          if (*q != 'a')
            abort();

        for (i = 0, c = 'A' + off2; i < len; i++, q++, c++) {
          if (c >= 'A' + SEQUENCE_LENGTH)
            c = 'A';
          if (*q != c)
            abort();
        }

        for (i = 0; i < MAX_EXTRA; i++, q++)
          if (*q != 'a')
            abort();
      }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test memcpy with various combinations of pointer alignments and lengths to
// CHECK-NEXT: //! make sure any optimizations in the library are correct.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Michael Meissner, March 9, 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
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
// CHECK-NEXT: // Use a sequence length that is not divisible by two, to make it more
// CHECK-NEXT: // likely to detect when words are mixed up.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     buf: [i8; 96],
// CHECK-NEXT:     align_int: i64,
// CHECK-NEXT:     align_fp: LongDouble,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u1: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut u2: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut off1: i32 = 0 as i32;
// CHECK-NEXT:     let mut off2: i32 = 0 as i32;
// CHECK-NEXT:     let mut len: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: i8 = 0 as i8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((off1 as i64) as u64) < (8 as u64) {
// CHECK-NEXT:         off2 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((off2 as i64) as u64) < (8 as u64) {
// CHECK-NEXT:             len = 1 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while ((len as i64) as u64)
// CHECK-NEXT:                 < (((10 as i32) as i64) as u64).wrapping_mul(8 as u64)
// CHECK-NEXT:             {
// CHECK-NEXT:                 i = 0 as i32;
// CHECK-NEXT:                 c = (65 as i32) as i8;
// CHECK-NEXT:                 '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while ((i as i64) as u64)
// CHECK-NEXT:                     < (8 as u64)
// CHECK-NEXT:                         .wrapping_add((((10 as i32) as i64) as u64).wrapping_mul(8 as u64))
// CHECK-NEXT:                         .wrapping_add(8 as u64)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(u1.buf) as *mut i8 }.offset(i as isize)
// CHECK-NEXT:                         } = (97 as i32) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (c as i32) >= (65 as i32) + (31 as i32) {
// CHECK-NEXT:                         c = (65 as i32) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(u2.buf) as *mut i8 }.offset(i as isize)
// CHECK-NEXT:                         } = c;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:                     i = [[V_2]];
// CHECK-NEXT:                     let mut [[V_3:__v[0-9]+]]: i8 = c;
// CHECK-NEXT:                     let mut [[V_4:__v[0-9]+]]: i8 = (([[V_3]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:                     c = [[V_4]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 p = (unsafe {
// CHECK-NEXT:                     memcpy(
// CHECK-NEXT:                         (unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(u1.buf) as *mut i8 }
// CHECK-NEXT:                                 .offset(off1 as isize)
// CHECK-NEXT:                         }) as *mut (),
// CHECK-NEXT:                         (unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(u2.buf) as *mut i8 }
// CHECK-NEXT:                                 .offset(off2 as isize)
// CHECK-NEXT:                         }) as *const (),
// CHECK-NEXT:                         (len as i64) as u64,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 }) as *mut i8;
// CHECK-NEXT:                 if p != unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(u1.buf) as *mut i8 }.offset(off1 as isize)
// CHECK-NEXT:                 } {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 q = unsafe { std::ptr::addr_of_mut!(u1.buf) as *mut i8 };
// CHECK-NEXT:                 i = 0 as i32;
// CHECK-NEXT:                 '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < off1 {
// CHECK-NEXT:                     if ((unsafe { *q }) as i32) != (97 as i32) {
// CHECK-NEXT:                         unsafe { abort() };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                     let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:                     i = [[V_6]];
// CHECK-NEXT:                     let mut [[V_7:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:                     let mut [[V_8:__v[0-9]+]]: *mut i8 = unsafe { [[V_7]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     q = [[V_8]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 i = 0 as i32;
// CHECK-NEXT:                 let mut [[V_9:__v[0-9]+]]: i8 = ((65 as i32) + off2) as i8;
// CHECK-NEXT:                 c = [[V_9]];
// CHECK-NEXT:                 '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:                     if (c as i32) >= (65 as i32) + (31 as i32) {
// CHECK-NEXT:                         c = (65 as i32) as i8;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if ((unsafe { *q }) as i32) != (c as i32) {
// CHECK-NEXT:                         unsafe { abort() };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_10:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                     let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:                     i = [[V_11]];
// CHECK-NEXT:                     let mut [[V_12:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:                     let mut [[V_13:__v[0-9]+]]: *mut i8 = unsafe { [[V_12]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     q = [[V_13]];
// CHECK-NEXT:                     let mut [[V_14:__v[0-9]+]]: i8 = c;
// CHECK-NEXT:                     let mut [[V_15:__v[0-9]+]]: i8 = (([[V_14]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:                     c = [[V_15]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 i = 0 as i32;
// CHECK-NEXT:                 '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (8 as u64) {
// CHECK-NEXT:                     if ((unsafe { *q }) as i32) != (97 as i32) {
// CHECK-NEXT:                         unsafe { abort() };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_16:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                     let mut [[V_17:__v[0-9]+]]: i32 = [[V_16]] + (1 as i32);
// CHECK-NEXT:                     i = [[V_17]];
// CHECK-NEXT:                     let mut [[V_18:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:                     let mut [[V_19:__v[0-9]+]]: *mut i8 = unsafe { [[V_18]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     q = [[V_19]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_20:__v[0-9]+]]: i32 = len;
// CHECK-NEXT:                 let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (1 as i32);
// CHECK-NEXT:                 len = [[V_21]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_22:__v[0-9]+]]: i32 = off2;
// CHECK-NEXT:             let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] + (1 as i32);
// CHECK-NEXT:             off2 = [[V_23]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: i32 = off1;
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: i32 = [[V_24]] + (1 as i32);
// CHECK-NEXT:         off1 = [[V_25]];
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
