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
// DEFAULT-NEXT:     fn %[[VALUE_testc:[0-9]+]] @testc(%[[VALUE_c:[0-9]+]] c: u8, %[[VALUE_ok:[0-9]+]] ok: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c]]))), const<i32>(1)), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c]]))), const<i32>(127)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests:[0-9]+]] @tests(%[[VALUE_s:[0-9]+]] s: u16, %[[VALUE_ok_2:[0-9]+]] ok: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(ge<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s]]))), const<i32>(1)), le<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_s]]))), const<i32>(32767)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_2]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testi:[0-9]+]] @testi(%[[VALUE_i:[0-9]+]] i: u32, %[[VALUE_ok_3:[0-9]+]] ok: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(ge<u32>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), le<u32>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_3]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl(%[[VALUE_l:[0-9]+]] l: u64, %[[VALUE_ok_4:[0-9]+]] ok: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(ge<u64>(read<u64>(%[[VALUE_l]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), le<u64>(read<u64>(%[[VALUE_l]]), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_4]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_4]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_testc]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_testc]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_testc]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(127))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_testc]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(const<i32>(127), const<i32>(1)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u8, i32) -> void>(%[[VALUE_testc]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_tests]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_tests]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_tests]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(32767))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_tests]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(const<i32>(32767), const<i32>(1)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u16, i32) -> void>(%[[VALUE_tests]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(32767), const<i32>(2)), const<i32>(1)))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_testi]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_testi]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_testi]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_testi]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(1)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u32, i32) -> void>(%[[VALUE_testi]], add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_testl]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_testl]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_testl]], reinterpret<u64, reason=arg, fits=always>(const<i64>(9223372036854775807)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_testl]], add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(1)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32) -> void>(%[[VALUE_testl]], add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), const<u64>(1)), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
