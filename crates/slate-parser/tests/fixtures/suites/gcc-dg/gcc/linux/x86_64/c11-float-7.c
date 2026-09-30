/* Test C11 definition of LDBL_EPSILON.  Based on
   gcc.target/powerpc/rs6000-ldouble-2.c.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <float.h>

extern void abort (void);
extern void exit (int);

int
main (void)
{
  volatile long double ee = 1.0;
  long double eps = ee;
  while (ee + 1.0 != 1.0)
    {
      eps = ee;
      ee = eps / 2;
    }
  if (eps != LDBL_EPSILON)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ee:[0-9]+]] ee: volatile f80 [storage=automatic] = float_widen<f80, reason=assign>(const<f64>(1.0));
// DEFAULT-NEXT:         let %[[VALUE_eps:[0-9]+]] eps: f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_ee]]);
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] ne<f80, exceptions=observable>(add<f80, rounding=nearest_even, exceptions=observable, contract=off>(read<f80, volatile>(%[[VALUE_ee]]), float_widen<f80, reason=usual_arith>(const<f64>(1.0))), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f80>(%[[VALUE_eps]], read<f80, volatile>(%[[VALUE_ee]]));
// DEFAULT-NEXT:                 write<f80, volatile>(%[[VALUE_ee]], div<f80, rounding=nearest_even, exceptions=observable, contract=off>(read<f80>(%[[VALUE_eps]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE_eps]]), const<f80>(1.08420217248550443401E-19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
