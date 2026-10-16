/* Copyright (C) 2002  Free Software Foundation.

   Test that optimizing ((c>=1) && (c<=127)) into (signed char)c < 0
   doesn't cause any problems for the compiler and behaves correctly.

   Written by Roger Sayle, 8th May 2002.  */

#include <limits.h>

extern void abort(void);

void testc(unsigned char c, int ok) {
  if ((c >= 1) && (c <= SCHAR_MAX)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

void tests(unsigned short s, int ok) {
  if ((s >= 1) && (s <= SHRT_MAX)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

void testi(unsigned int i, int ok) {
  if ((i >= 1) && (i <= INT_MAX)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

void testl(unsigned long l, int ok) {
  if ((l >= 1) && (l <= LONG_MAX)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int main() {
  testc(0, 0);
  testc(1, 1);
  testc(SCHAR_MAX, 1);
  testc(SCHAR_MAX + 1, 0);
  testc(UCHAR_MAX, 0);

  tests(0, 0);
  tests(1, 1);
  tests(SHRT_MAX, 1);
  tests(SHRT_MAX + 1, 0);
  tests(USHRT_MAX, 0);

  testi(0, 0);
  testi(1, 1);
  testi(INT_MAX, 1);
  testi(INT_MAX + 1U, 0);
  testi(UINT_MAX, 0);

  testl(0, 0);
  testl(1, 1);
  testl(LONG_MAX, 1);
  testl(LONG_MAX + 1UL, 0);
  testl(ULONG_MAX, 0);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test that optimizing ((c>=1) && (c<=127)) into (signed char)c < 0
// CHECK-NEXT: //! doesn't cause any problems for the compiler and behaves correctly.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 8th May 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testc(mut c: u8, mut ok: i32) {
// CHECK-NEXT:     if ((c as u32) as i32) >= (1 as i32) && ((c as u32) as i32) <= (127 as i32) {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn tests(mut s: u16, mut ok: i32) {
// CHECK-NEXT:     if ((s as u32) as i32) >= (1 as i32) && ((s as u32) as i32) <= (32767 as i32) {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testi(mut i: u32, mut ok: i32) {
// CHECK-NEXT:     if i >= ((1 as i32) as u32) && i <= ((2147483647 as i32) as u32) {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn testl(mut l: u64, mut ok: i32) {
// CHECK-NEXT:     if l >= (((1 as i32) as i64) as u64) && l <= ((9223372036854775807 as i64) as u64) {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     testc(((0 as i32) as i8) as u8, 0 as i32);
// CHECK-NEXT:     testc(((1 as i32) as i8) as u8, 1 as i32);
// CHECK-NEXT:     testc(((127 as i32) as i8) as u8, 1 as i32);
// CHECK-NEXT:     testc((((127 as i32) + (1 as i32)) as i8) as u8, 0 as i32);
// CHECK-NEXT:     testc(
// CHECK-NEXT:         (((127 as i32) * (2 as i32) + (1 as i32)) as i8) as u8,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     tests(((0 as i32) as i16) as u16, 0 as i32);
// CHECK-NEXT:     tests(((1 as i32) as i16) as u16, 1 as i32);
// CHECK-NEXT:     tests(((32767 as i32) as i16) as u16, 1 as i32);
// CHECK-NEXT:     tests((((32767 as i32) + (1 as i32)) as i16) as u16, 0 as i32);
// CHECK-NEXT:     tests(
// CHECK-NEXT:         (((32767 as i32) * (2 as i32) + (1 as i32)) as i16) as u16,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     testi((0 as i32) as u32, 0 as i32);
// CHECK-NEXT:     testi((1 as i32) as u32, 1 as i32);
// CHECK-NEXT:     testi((2147483647 as i32) as u32, 1 as i32);
// CHECK-NEXT:     testi(
// CHECK-NEXT:         ((2147483647 as i32) as u32).wrapping_add(1 as u32),
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     testi(
// CHECK-NEXT:         ((2147483647 as i32) as u32)
// CHECK-NEXT:             .wrapping_mul(2 as u32)
// CHECK-NEXT:             .wrapping_add(1 as u32),
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     testl(((0 as i32) as i64) as u64, 0 as i32);
// CHECK-NEXT:     testl(((1 as i32) as i64) as u64, 1 as i32);
// CHECK-NEXT:     testl((9223372036854775807 as i64) as u64, 1 as i32);
// CHECK-NEXT:     testl(
// CHECK-NEXT:         ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64),
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     testl(
// CHECK-NEXT:         ((9223372036854775807 as i64) as u64)
// CHECK-NEXT:             .wrapping_mul(2 as u64)
// CHECK-NEXT:             .wrapping_add(1 as u64),
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     0 as i32
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
