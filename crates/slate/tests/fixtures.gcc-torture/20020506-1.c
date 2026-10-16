/* Copyright (C) 2002  Free Software Foundation.

   Test that (A & C1) op C2 optimizations behave correctly where C1 is
   a constant power of 2, op is == or !=, and C2 is C1 or zero.

   Written by Roger Sayle, 5th May 2002.  */

#include <limits.h>

extern void abort(void);

void test1(signed char c, int set);
void test2(unsigned char c, int set);
void test3(short s, int set);
void test4(unsigned short s, int set);
void test5(int i, int set);
void test6(unsigned int i, int set);
void test7(long long l, int set);
void test8(unsigned long long l, int set);

#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX __LONG_LONG_MAX__
#endif
#ifndef LONG_LONG_MIN
#define LONG_LONG_MIN (-LONG_LONG_MAX - 1)
#endif
#ifndef ULONG_LONG_MAX
#define ULONG_LONG_MAX (LONG_LONG_MAX * 2ULL + 1)
#endif

void test1(signed char c, int set) {
  if ((c & (SCHAR_MAX + 1)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((c & (SCHAR_MAX + 1)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((c & (SCHAR_MAX + 1)) == (SCHAR_MAX + 1)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((c & (SCHAR_MAX + 1)) != (SCHAR_MAX + 1)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test2(unsigned char c, int set) {
  if ((c & (SCHAR_MAX + 1)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((c & (SCHAR_MAX + 1)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((c & (SCHAR_MAX + 1)) == (SCHAR_MAX + 1)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((c & (SCHAR_MAX + 1)) != (SCHAR_MAX + 1)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test3(short s, int set) {
  if ((s & (SHRT_MAX + 1)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((s & (SHRT_MAX + 1)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((s & (SHRT_MAX + 1)) == (SHRT_MAX + 1)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((s & (SHRT_MAX + 1)) != (SHRT_MAX + 1)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test4(unsigned short s, int set) {
  if ((s & (SHRT_MAX + 1)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((s & (SHRT_MAX + 1)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((s & (SHRT_MAX + 1)) == (SHRT_MAX + 1)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((s & (SHRT_MAX + 1)) != (SHRT_MAX + 1)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test5(int i, int set) {
  if ((i & (INT_MAX + 1U)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((i & (INT_MAX + 1U)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((i & (INT_MAX + 1U)) == (INT_MAX + 1U)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((i & (INT_MAX + 1U)) != (INT_MAX + 1U)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test6(unsigned int i, int set) {
  if ((i & (INT_MAX + 1U)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((i & (INT_MAX + 1U)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((i & (INT_MAX + 1U)) == (INT_MAX + 1U)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((i & (INT_MAX + 1U)) != (INT_MAX + 1U)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test7(long long l, int set) {
  if ((l & (LONG_LONG_MAX + 1ULL)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((l & (LONG_LONG_MAX + 1ULL)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((l & (LONG_LONG_MAX + 1ULL)) == (LONG_LONG_MAX + 1ULL)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((l & (LONG_LONG_MAX + 1ULL)) != (LONG_LONG_MAX + 1ULL)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

void test8(unsigned long long l, int set) {
  if ((l & (LONG_LONG_MAX + 1ULL)) == 0) {
    if (set)
      abort();
  } else if (!set)
    abort();

  if ((l & (LONG_LONG_MAX + 1ULL)) != 0) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((l & (LONG_LONG_MAX + 1ULL)) == (LONG_LONG_MAX + 1ULL)) {
    if (!set)
      abort();
  } else if (set)
    abort();

  if ((l & (LONG_LONG_MAX + 1ULL)) != (LONG_LONG_MAX + 1ULL)) {
    if (set)
      abort();
  } else if (!set)
    abort();
}

int main() {
  test1(0, 0);
  test1(SCHAR_MAX, 0);
  test1(SCHAR_MIN, 1);
  test1(UCHAR_MAX, 1);

  test2(0, 0);
  test2(SCHAR_MAX, 0);
  test2(SCHAR_MIN, 1);
  test2(UCHAR_MAX, 1);

  test3(0, 0);
  test3(SHRT_MAX, 0);
  test3(SHRT_MIN, 1);
  test3(USHRT_MAX, 1);

  test4(0, 0);
  test4(SHRT_MAX, 0);
  test4(SHRT_MIN, 1);
  test4(USHRT_MAX, 1);

  test5(0, 0);
  test5(INT_MAX, 0);
  test5(INT_MIN, 1);
  test5(UINT_MAX, 1);

  test6(0, 0);
  test6(INT_MAX, 0);
  test6(INT_MIN, 1);
  test6(UINT_MAX, 1);

  test7(0, 0);
  test7(LONG_LONG_MAX, 0);
  test7(LONG_LONG_MIN, 1);
  test7(ULONG_LONG_MAX, 1);

  test8(0, 0);
  test8(LONG_LONG_MAX, 0);
  test8(LONG_LONG_MIN, 1);
  test8(ULONG_LONG_MAX, 1);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test that (A & C1) op C2 optimizations behave correctly where C1 is
// CHECK-NEXT: //! a constant power of 2, op is == or !=, and C2 is C1 or zero.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 5th May 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut c: i8, mut set: i32) {
// CHECK-NEXT:     if (c as i32) & (127 as i32) + (1 as i32) == (0 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (c as i32) & (127 as i32) + (1 as i32) != (0 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (c as i32) & (127 as i32) + (1 as i32) == (127 as i32) + (1 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (c as i32) & (127 as i32) + (1 as i32) != (127 as i32) + (1 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut c: u8, mut set: i32) {
// CHECK-NEXT:     if ((c as u32) as i32) & (127 as i32) + (1 as i32) == (0 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((c as u32) as i32) & (127 as i32) + (1 as i32) != (0 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((c as u32) as i32) & (127 as i32) + (1 as i32) == (127 as i32) + (1 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((c as u32) as i32) & (127 as i32) + (1 as i32) != (127 as i32) + (1 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut s: i16, mut set: i32) {
// CHECK-NEXT:     if (s as i32) & (32767 as i32) + (1 as i32) == (0 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (s as i32) & (32767 as i32) + (1 as i32) != (0 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (s as i32) & (32767 as i32) + (1 as i32) == (32767 as i32) + (1 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (s as i32) & (32767 as i32) + (1 as i32) != (32767 as i32) + (1 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut s: u16, mut set: i32) {
// CHECK-NEXT:     if ((s as u32) as i32) & (32767 as i32) + (1 as i32) == (0 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((s as u32) as i32) & (32767 as i32) + (1 as i32) != (0 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((s as u32) as i32) & (32767 as i32) + (1 as i32) == (32767 as i32) + (1 as i32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((s as u32) as i32) & (32767 as i32) + (1 as i32) != (32767 as i32) + (1 as i32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut i: i32, mut set: i32) {
// CHECK-NEXT:     if (i as u32) & ((2147483647 as i32) as u32).wrapping_add(1 as u32) == ((0 as i32) as u32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (i as u32) & ((2147483647 as i32) as u32).wrapping_add(1 as u32) != ((0 as i32) as u32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (i as u32) & ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:         == ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (i as u32) & ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:         != ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut i: u32, mut set: i32) {
// CHECK-NEXT:     if i & ((2147483647 as i32) as u32).wrapping_add(1 as u32) == ((0 as i32) as u32) {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if i & ((2147483647 as i32) as u32).wrapping_add(1 as u32) != ((0 as i32) as u32) {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if i & ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:         == ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if i & ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:         != ((2147483647 as i32) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test7(mut l: i64, mut set: i32) {
// CHECK-NEXT:     if (l as u64) & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (l as u64) & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         != (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (l as u64) & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         == ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (l as u64) & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         != ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test8(mut l: u64, mut set: i32) {
// CHECK-NEXT:     if l & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         == (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if l & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         != (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if l & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         == ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if l & ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:         != ((9223372036854775807 as i64) as u64).wrapping_add(1 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if set != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !(set != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1((0 as i32) as i8, 0 as i32);
// CHECK-NEXT:     test1((127 as i32) as i8, 0 as i32);
// CHECK-NEXT:     test1((-(127 as i32) - (1 as i32)) as i8, 1 as i32);
// CHECK-NEXT:     test1(((127 as i32) * (2 as i32) + (1 as i32)) as i8, 1 as i32);
// CHECK-NEXT:     test2(((0 as i32) as i8) as u8, 0 as i32);
// CHECK-NEXT:     test2(((127 as i32) as i8) as u8, 0 as i32);
// CHECK-NEXT:     test2(((-(127 as i32) - (1 as i32)) as i8) as u8, 1 as i32);
// CHECK-NEXT:     test2(
// CHECK-NEXT:         (((127 as i32) * (2 as i32) + (1 as i32)) as i8) as u8,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test3((0 as i32) as i16, 0 as i32);
// CHECK-NEXT:     test3((32767 as i32) as i16, 0 as i32);
// CHECK-NEXT:     test3((-(32767 as i32) - (1 as i32)) as i16, 1 as i32);
// CHECK-NEXT:     test3(((32767 as i32) * (2 as i32) + (1 as i32)) as i16, 1 as i32);
// CHECK-NEXT:     test4(((0 as i32) as i16) as u16, 0 as i32);
// CHECK-NEXT:     test4(((32767 as i32) as i16) as u16, 0 as i32);
// CHECK-NEXT:     test4(((-(32767 as i32) - (1 as i32)) as i16) as u16, 1 as i32);
// CHECK-NEXT:     test4(
// CHECK-NEXT:         (((32767 as i32) * (2 as i32) + (1 as i32)) as i16) as u16,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test5(0 as i32, 0 as i32);
// CHECK-NEXT:     test5(2147483647 as i32, 0 as i32);
// CHECK-NEXT:     test5(-(2147483647 as i32) - (1 as i32), 1 as i32);
// CHECK-NEXT:     test5(
// CHECK-NEXT:         ((2147483647 as i32) as u32)
// CHECK-NEXT:             .wrapping_mul(2 as u32)
// CHECK-NEXT:             .wrapping_add(1 as u32) as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test6((0 as i32) as u32, 0 as i32);
// CHECK-NEXT:     test6((2147483647 as i32) as u32, 0 as i32);
// CHECK-NEXT:     test6((-(2147483647 as i32) - (1 as i32)) as u32, 1 as i32);
// CHECK-NEXT:     test6(
// CHECK-NEXT:         ((2147483647 as i32) as u32)
// CHECK-NEXT:             .wrapping_mul(2 as u32)
// CHECK-NEXT:             .wrapping_add(1 as u32),
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test7((0 as i32) as i64, 0 as i32);
// CHECK-NEXT:     test7(9223372036854775807 as i64, 0 as i32);
// CHECK-NEXT:     test7(
// CHECK-NEXT:         -(9223372036854775807 as i64) - ((1 as i32) as i64),
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test7(
// CHECK-NEXT:         ((9223372036854775807 as i64) as u64)
// CHECK-NEXT:             .wrapping_mul(2 as u64)
// CHECK-NEXT:             .wrapping_add(((1 as i32) as i64) as u64) as i64,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test8(((0 as i32) as i64) as u64, 0 as i32);
// CHECK-NEXT:     test8((9223372036854775807 as i64) as u64, 0 as i32);
// CHECK-NEXT:     test8(
// CHECK-NEXT:         (-(9223372036854775807 as i64) - ((1 as i32) as i64)) as u64,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test8(
// CHECK-NEXT:         ((9223372036854775807 as i64) as u64)
// CHECK-NEXT:             .wrapping_mul(2 as u64)
// CHECK-NEXT:             .wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:         1 as i32,
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
