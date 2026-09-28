/* PR target/96307 */
/* { dg-do compile } */
/* { dg-additional-options "-fsanitize=kernel-address --param=asan-instrumentation-with-call-threshold=8" } */
/* { dg-skip-if "no address sanitizer" { no_fsanitize_address } } */

#include <limits.h>
enum a {test1, test2, test3=INT_MAX};
enum a a;
enum a *b;

void reset (void);

void
t()
{
  if (a != test2)
    __builtin_abort ();
  if (*b != test2)
    __builtin_abort ();
  reset ();
  if (a != test1)
    __builtin_abort ();
  if (*b != test1)
    __builtin_abort ();
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
// DEFAULT-NEXT:     type @type0 a = enum : u32 {
// DEFAULT-NEXT:         %0 test1 = const<i32>(0);
// DEFAULT-NEXT:         %1 test2 = const<i32>(1);
// DEFAULT-NEXT:         %2 test3 = const<i32>(2147483647);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %4 a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 b: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @reset() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @t() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(%4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(deref(read<ptr<@type0>>(%5)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(%4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(deref(read<ptr<@type0>>(%5)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
