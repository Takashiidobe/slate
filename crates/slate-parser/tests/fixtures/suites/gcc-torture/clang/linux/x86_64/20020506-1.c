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



// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @test1(%25 c: i8, %26 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%25)), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%25)), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%25)), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%25)), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%26), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2(%27 c: u8, %28 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%28), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%28), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%28), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%28), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%28), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%28), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%28), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%28), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test3(%29 s: i16, %30 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%29)), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%30), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%30), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%29)), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%30), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%30), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%29)), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%30), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%30), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%29)), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%30), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%30), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test4(%31 s: u16, %32 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%32), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%32), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%32), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%32), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%32), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%32), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%31))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%32), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%32), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test5(%33 i: i32, %34 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%33)), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%34), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%34), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%33)), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%34), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%34), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%33)), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%34), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%34), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%33)), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%34), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%34), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test6(%35 i: u32, %36 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(and<u32>(read<u32>(%35), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%36), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%36), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(read<u32>(%35), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%36), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%36), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<u32>(and<u32>(read<u32>(%35), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%36), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%36), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(read<u32>(%35), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%36), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%36), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test7(%37 l: i64, %38 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%37)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%38), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%38), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%37)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%38), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%38), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%37)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%38), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%38), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%37)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%38), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%38), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test8(%39 l: u64, %40 set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(read<u64>(%39), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%40), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(read<u64>(%39), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%40), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<u64>(and<u64>(read<u64>(%39), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%40), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(read<u64>(%39), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%40), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%3, truncate<i8, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%3, truncate<i8, reason=arg, fits=always>(const<i32>(127)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%3, truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%3, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%6, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%6, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(127))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%6, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%6, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%9, truncate<i16, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%9, truncate<i16, reason=arg, fits=always>(const<i32>(32767)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%9, truncate<i16, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%9, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%12, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%12, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(32767))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%12, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%12, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%15, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%15, const<i32>(2147483647), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%15, sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%15, reinterpret<i32, reason=arg, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%18, reinterpret<u32, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%18, add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%21, widen<i64, reason=arg>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%21, const<i64>(9223372036854775807), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%21, sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%21, reinterpret<i64, reason=arg, fits=unknown>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%24, reinterpret<u64, reason=arg, fits=always>(const<i64>(9223372036854775807)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%24, reinterpret<u64, reason=arg, fits=unknown>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%24, add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
