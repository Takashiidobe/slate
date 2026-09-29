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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_c:[0-9]+]] c: i8, %[[VALUE_set:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_c_2:[0-9]+]] c: u8, %[[VALUE_set_2:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c_2]]))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c_2]]))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c_2]]))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c_2]]))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1))), add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_2]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_s:[0-9]+]] s: i16, %[[VALUE_set_3:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_3]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_s_2:[0-9]+]] s: u16, %[[VALUE_set_4:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s_2]]))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s_2]]))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s_2]]))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s_2]]))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1))), add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_4]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_set_5:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_i]])), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_i]])), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_i]])), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_i]])), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_5]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_i_2:[0-9]+]] i: u32, %[[VALUE_set_6:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(and<u32>(read<u32>(%[[VALUE_i_2]]), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(read<u32>(%[[VALUE_i_2]]), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<u32>(and<u32>(read<u32>(%[[VALUE_i_2]]), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(and<u32>(read<u32>(%[[VALUE_i_2]]), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1))), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_6]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_set_7:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_l]])), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_l]])), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_l]])), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_l]])), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_7]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_l_2:[0-9]+]] l: u64, %[[VALUE_set_8:[0-9]+]] set: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(read<u64>(%[[VALUE_l_2]]), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(read<u64>(%[[VALUE_l_2]]), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<u64>(and<u64>(read<u64>(%[[VALUE_l_2]]), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(and<u64>(read<u64>(%[[VALUE_l_2]]), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_set_8]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%[[VALUE_test1]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%[[VALUE_test1]], truncate<i8, reason=arg, fits=always>(const<i32>(127)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%[[VALUE_test1]], truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i32) -> void>(%[[VALUE_test1]], truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_test2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_test2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(127))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_test2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_test2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%[[VALUE_test3]], truncate<i16, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%[[VALUE_test3]], truncate<i16, reason=arg, fits=always>(const<i32>(32767)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%[[VALUE_test3]], truncate<i16, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i16, i32) -> void>(%[[VALUE_test3]], truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_test4]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_test4]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(32767))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_test4]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_test4]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test5]], const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test5]], const<i32>(2147483647), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test5]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_test5]], reinterpret<i32, reason=arg, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_test6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_test6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_test6]], reinterpret<u32, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_test6]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%[[VALUE_test7]], widen<i64, reason=arg>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%[[VALUE_test7]], const<i64>(9223372036854775807), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%[[VALUE_test7]], sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(%[[VALUE_test7]], reinterpret<i64, reason=arg, fits=unknown>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_test8]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_test8]], reinterpret<u64, reason=arg, fits=always>(const<i64>(9223372036854775807)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_test8]], reinterpret<u64, reason=arg, fits=unknown>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_test8]], add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
