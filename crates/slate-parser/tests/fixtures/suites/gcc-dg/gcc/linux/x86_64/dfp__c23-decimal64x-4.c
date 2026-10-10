/* Test _Decimal64x in C23 mode - float.h macros.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#ifndef __STDC_WANT_IEC_60559_TYPES_EXT__
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#endif

#include <float.h>

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)
static_assert (expr_has_type (DEC64X_MAX, _Decimal64x));
static_assert (expr_has_type (DEC64X_MIN, _Decimal64x));
static_assert (expr_has_type (DEC64X_EPSILON, _Decimal64x));
static_assert (expr_has_type (DEC64X_TRUE_MIN, _Decimal64x));

int
main ()
{
#if __DEC64X_MANT_DIG__ == __DEC128_MANT_DIG__ \
    && __DEC64X_MAX_EXP__ == __DEC128_MAX_EXP__
  if (DEC64X_MANT_DIG != 34)
    __builtin_abort ();

  if (DEC64X_MIN_EXP != -6142)
    __builtin_abort ();

  if (DEC64X_MAX_EXP != 6145)
    __builtin_abort ();

  if (DEC64X_MAX != 9.999999999999999999999999999999999E6144D64x)
    __builtin_abort ();

  if (DEC64X_EPSILON != 1E-33D64x)
    __builtin_abort ();
  
  if (DEC64X_MIN != 1E-6143D64x)
    __builtin_abort ();

  if (DEC64X_TRUE_MIN != 0.000000000000000000000000000000001E-6143D64x)
    __builtin_abort ();
#endif
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(34), const<i32>(34))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(neg<i32, overflow=ub>(const<i32>(6142)), neg<i32, overflow=ub>(const<i32>(6142)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(6145), const<i32>(6145))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(const<d128>(9.999999999999999999999999999999999e6144), const<d128>(9.999999999999999999999999999999999e6144))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(const<d128>(1e-33), const<d128>(1e-33))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(const<d128>(1e-6143), const<d128>(1e-6143))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(const<d128>(0.000000000000000000000000000000001e-6143), const<d128>(0.000000000000000000000000000000001e-6143))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
