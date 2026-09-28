
/* { dg-do run  } */
/* { dg-options "-O0" } */

#include <limits.h>
#define ABS(x)	(((x) >= 0) ? (x) : -(x))

#define DEF_TEST(TYPE)	\
void foo_##TYPE (signed TYPE x, unsigned TYPE y){	\
    TYPE t = ABS (x);				\
    if (t != y)					\
 	__builtin_abort ();			\
}						\

DEF_TEST (char);
DEF_TEST (short);
DEF_TEST (int);
DEF_TEST (long);

int main ()
{
  foo_char (SCHAR_MIN + 1, SCHAR_MAX);
  foo_char (0, 0);
  foo_char (-1, 1);
  foo_char (1, 1);
  foo_char (SCHAR_MAX, SCHAR_MAX);

  foo_int (-1, 1);
  foo_int (0, 0);
  foo_int (INT_MAX, INT_MAX);
  foo_int (INT_MIN + 1, INT_MAX);

  foo_short (-1, 1);
  foo_short (0, 0);
  foo_short (SHRT_MAX, SHRT_MAX);
  foo_short (SHRT_MIN + 1, SHRT_MAX);

  foo_long (-1, 1);
  foo_long (0, 0);
  foo_long (LONG_MAX, LONG_MAX);
  foo_long (LONG_MIN + 1, LONG_MAX);

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %17 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @foo_char(%1 x: i8, %2 y: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 t: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%1)), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%1)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%3)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo_short(%5 x: i16, %6 y: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 t: i16 [storage=automatic] = truncate<i16, reason=assign, fits=unknown>(conditional<i32>(ge<i32>(widen<i32, reason=promotion>(read<i16>(%5)), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%5)), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%5)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%7)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo_int(%9 x: i32, %10 y: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 t: i32 [storage=automatic] = conditional<i32>(ge<i32>(read<i32>(%9), const<i32>(0)), read<i32>(%9), neg<i32, overflow=ub>(read<i32>(%9)));
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%11)), read<u32>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @foo_long(%13 x: i64, %14 y: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 t: i64 [storage=automatic] = conditional<i64>(ge<i64>(read<i64>(%13), widen<i64, reason=usual_arith>(const<i32>(0))), read<i64>(%13), neg<i64, overflow=ub>(read<i64>(%13)));
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%15)), read<u64>(%14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i8, u8) -> void>(%0, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)), const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(127))));
// DEFAULT-NEXT:         call<void, signature=fn(i8, u8) -> void>(%0, truncate<i8, reason=arg, fits=always>(const<i32>(0)), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(i8, u8) -> void>(%0, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i8, u8) -> void>(%0, truncate<i8, reason=arg, fits=always>(const<i32>(1)), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i8, u8) -> void>(%0, truncate<i8, reason=arg, fits=always>(const<i32>(127)), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(127))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, u32) -> void>(%8, neg<i32, overflow=ub>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, u32) -> void>(%8, const<i32>(0), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, u32) -> void>(%8, const<i32>(2147483647), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, u32) -> void>(%8, add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647)));
// DEFAULT-NEXT:         call<void, signature=fn(i16, u16) -> void>(%4, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i16, u16) -> void>(%4, truncate<i16, reason=arg, fits=always>(const<i32>(0)), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(i16, u16) -> void>(%4, truncate<i16, reason=arg, fits=always>(const<i32>(32767)), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(32767))));
// DEFAULT-NEXT:         call<void, signature=fn(i16, u16) -> void>(%4, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(32767))));
// DEFAULT-NEXT:         call<void, signature=fn(i64, u64) -> void>(%12, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i64, u64) -> void>(%12, widen<i64, reason=arg>(const<i32>(0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(i64, u64) -> void>(%12, const<i64>(9223372036854775807), reinterpret<u64, reason=arg, fits=always>(const<i64>(9223372036854775807)));
// DEFAULT-NEXT:         call<void, signature=fn(i64, u64) -> void>(%12, add<i64, overflow=ub>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1)), widen<i64, reason=usual_arith>(const<i32>(1))), reinterpret<u64, reason=arg, fits=always>(const<i64>(9223372036854775807)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
