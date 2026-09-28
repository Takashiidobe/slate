/* Tests for _FloatN / _FloatNx types: test conversions.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float16 } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float128 } */
/* { dg-add-options float32x } */
/* { dg-add-options float64x } */
/* { dg-add-options float128x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target floatn_nx_runtime } */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

#ifndef FLT16_MAX
# define _Float16 _Float32
# define FLT16_MAX FLT32_MAX
# define FLT16_MANT_DIG FLT32_MANT_DIG
# define FLT16_EPSILON FLT32_EPSILON
#endif

#ifndef FLT64_MAX
# define _Float64 _Float32
# define FLT64_MAX FLT32_MAX
# define FLT64_MANT_DIG FLT32_MANT_DIG
# define FLT64_EPSILON FLT32_EPSILON
#endif

#ifndef FLT128_MAX
# define _Float128 _Float32
# define FLT128_MAX FLT32_MAX
# define FLT128_MANT_DIG FLT32_MANT_DIG
# define FLT128_EPSILON FLT32_EPSILON
#endif

#ifndef FLT32X_MAX
# define _Float32x _Float32
# define FLT32X_MAX FLT32_MAX
# define FLT32X_MANT_DIG FLT32_MANT_DIG
# define FLT32X_EPSILON FLT32_EPSILON
#endif

#ifndef FLT64X_MAX
# define _Float64x _Float32
# define FLT64X_MAX FLT32_MAX
# define FLT64X_MANT_DIG FLT32_MANT_DIG
# define FLT64X_EPSILON FLT32_EPSILON
#endif

#ifndef FLT128X_MAX
# define _Float128x _Float32
# define FLT128X_MAX FLT32_MAX
# define FLT128X_MANT_DIG FLT32_MANT_DIG
# define FLT128X_EPSILON FLT32_EPSILON
#endif

#define CONCATX(X, Y) X ## Y
#define CONCAT(X, Y) CONCATX (X, Y)

extern void exit (int);
extern void abort (void);

#define DO_TEST(TYPE1, PFX1, TYPE2, PFX2)			\
  do								\
    {								\
      volatile TYPE1 a = (TYPE1) 1 + CONCAT (PFX1, _EPSILON);	\
      volatile TYPE2 b = (TYPE2) a;				\
      volatile TYPE2 expected;					\
      if (CONCAT (PFX2, _MANT_DIG) < CONCAT (PFX1, _MANT_DIG))	\
	expected = (TYPE2) 1;					\
      else							\
	expected = (TYPE2) 1 + (TYPE2) CONCAT (PFX1, _EPSILON); \
      if (b != expected)					\
	abort ();						\
    }								\
  while (0)

#define DO_TEST1(TYPE1, PFX1)				\
  do							\
    {							\
      DO_TEST (TYPE1, PFX1, _Float16, FLT16);		\
      DO_TEST (TYPE1, PFX1, _Float32, FLT32);		\
      DO_TEST (TYPE1, PFX1, _Float64, FLT64);		\
      DO_TEST (TYPE1, PFX1, _Float128, FLT128);		\
      DO_TEST (TYPE1, PFX1, _Float32x, FLT32X);		\
      DO_TEST (TYPE1, PFX1, _Float64x, FLT64X);		\
      DO_TEST (TYPE1, PFX1, _Float128x, FLT128X);	\
    }							\
  while (0)

int
main (void)
{
  DO_TEST1 (_Float16, FLT16);
  DO_TEST1 (_Float32, FLT32);
  DO_TEST1 (_Float64, FLT64);
  DO_TEST1 (_Float128, FLT128);
  DO_TEST1 (_Float32x, FLT32X);
  DO_TEST1 (_Float64x, FLT64X);
  DO_TEST1 (_Float128x, FLT128X);
  exit (0);
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
// DEFAULT-NEXT:     fn %0 @exit(%150 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %151
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %152
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %3 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %4 b: volatile f16 [storage=automatic] = read<f16, volatile>(%3);
// DEFAULT-NEXT:                         let %5 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(11))
// DEFAULT-NEXT:                             write<f16, volatile>(%5, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%5, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4)));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%4), read<f16, volatile>(%5))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %153
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %6 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %7 b: volatile f32 [storage=automatic] = float_widen<f32, reason=explicit>(read<f16, volatile>(%6));
// DEFAULT-NEXT:                         let %8 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(11))
// DEFAULT-NEXT:                             write<f32, volatile>(%8, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%8, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f32, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%7), read<f32, volatile>(%8))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %154
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %9 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %10 b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f16, volatile>(%9));
// DEFAULT-NEXT:                         let %11 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(11))
// DEFAULT-NEXT:                             write<f64, volatile>(%11, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%11, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%10), read<f64, volatile>(%11))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %155
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %12 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %13 b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f16, volatile>(%12));
// DEFAULT-NEXT:                         let %14 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(11))
// DEFAULT-NEXT:                             write<f128, volatile>(%14, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%14, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%13), read<f128, volatile>(%14))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %156
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %15 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %16 b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f16, volatile>(%15));
// DEFAULT-NEXT:                         let %17 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(11))
// DEFAULT-NEXT:                             write<f64, volatile>(%17, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%17, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%16), read<f64, volatile>(%17))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %157
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %18 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %19 b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f16, volatile>(%18));
// DEFAULT-NEXT:                         let %20 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(11))
// DEFAULT-NEXT:                             write<f80, volatile>(%20, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%20, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%19), read<f80, volatile>(%20))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %158
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %21 a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %22 b: volatile f32 [storage=automatic] = float_widen<f32, reason=explicit>(read<f16, volatile>(%21));
// DEFAULT-NEXT:                         let %23 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(11))
// DEFAULT-NEXT:                             write<f32, volatile>(%23, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%23, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f32, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%22), read<f32, volatile>(%23))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %159
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %160
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %24 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %25 b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f32, volatile>(%24));
// DEFAULT-NEXT:                         let %26 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(24))
// DEFAULT-NEXT:                             write<f16, volatile>(%26, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%26, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%25), read<f16, volatile>(%26))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %161
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %27 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %28 b: volatile f32 [storage=automatic] = read<f32, volatile>(%27);
// DEFAULT-NEXT:                         let %29 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%29, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%29, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%28), read<f32, volatile>(%29))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %162
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %30 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %31 b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%30));
// DEFAULT-NEXT:                         let %32 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%32, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%32, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%31), read<f64, volatile>(%32))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %163
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %33 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %34 b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f32, volatile>(%33));
// DEFAULT-NEXT:                         let %35 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(24))
// DEFAULT-NEXT:                             write<f128, volatile>(%35, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%35, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%34), read<f128, volatile>(%35))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %164
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %36 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %37 b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%36));
// DEFAULT-NEXT:                         let %38 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%38, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%38, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%37), read<f64, volatile>(%38))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %165
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %39 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %40 b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f32, volatile>(%39));
// DEFAULT-NEXT:                         let %41 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(24))
// DEFAULT-NEXT:                             write<f80, volatile>(%41, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%41, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%40), read<f80, volatile>(%41))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %166
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %42 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %43 b: volatile f32 [storage=automatic] = read<f32, volatile>(%42);
// DEFAULT-NEXT:                         let %44 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%44, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%44, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%43), read<f32, volatile>(%44))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %167
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %168
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %45 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %46 b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%45));
// DEFAULT-NEXT:                         let %47 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(53))
// DEFAULT-NEXT:                             write<f16, volatile>(%47, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%47, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%46), read<f16, volatile>(%47))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %169
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %48 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %49 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%48));
// DEFAULT-NEXT:                         let %50 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%50, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%50, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%49), read<f32, volatile>(%50))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %170
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %51 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %52 b: volatile f64 [storage=automatic] = read<f64, volatile>(%51);
// DEFAULT-NEXT:                         let %53 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%53, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%53, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%52), read<f64, volatile>(%53))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %171
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %54 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %55 b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f64, volatile>(%54));
// DEFAULT-NEXT:                         let %56 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(53))
// DEFAULT-NEXT:                             write<f128, volatile>(%56, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%56, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%55), read<f128, volatile>(%56))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %172
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %57 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %58 b: volatile f64 [storage=automatic] = read<f64, volatile>(%57);
// DEFAULT-NEXT:                         let %59 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%59, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%59, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%58), read<f64, volatile>(%59))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %173
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %60 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %61 b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f64, volatile>(%60));
// DEFAULT-NEXT:                         let %62 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(53))
// DEFAULT-NEXT:                             write<f80, volatile>(%62, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%62, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%61), read<f80, volatile>(%62))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %174
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %63 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %64 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%63));
// DEFAULT-NEXT:                         let %65 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%65, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%65, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%64), read<f32, volatile>(%65))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %175
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %176
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %66 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %67 b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%66));
// DEFAULT-NEXT:                         let %68 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(113))
// DEFAULT-NEXT:                             write<f16, volatile>(%68, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%68, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%67), read<f16, volatile>(%68))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %177
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %69 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %70 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%69));
// DEFAULT-NEXT:                         let %71 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(113))
// DEFAULT-NEXT:                             write<f32, volatile>(%71, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%71, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%70), read<f32, volatile>(%71))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %178
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %72 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %73 b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%72));
// DEFAULT-NEXT:                         let %74 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(113))
// DEFAULT-NEXT:                             write<f64, volatile>(%74, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%74, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%73), read<f64, volatile>(%74))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %179
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %75 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %76 b: volatile f128 [storage=automatic] = read<f128, volatile>(%75);
// DEFAULT-NEXT:                         let %77 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(113))
// DEFAULT-NEXT:                             write<f128, volatile>(%77, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%77, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34)));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%76), read<f128, volatile>(%77))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %180
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %78 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %79 b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%78));
// DEFAULT-NEXT:                         let %80 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(113))
// DEFAULT-NEXT:                             write<f64, volatile>(%80, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%80, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%79), read<f64, volatile>(%80))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %181
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %81 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %82 b: volatile f80 [storage=automatic] = float_narrow<f80, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%81));
// DEFAULT-NEXT:                         let %83 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(113))
// DEFAULT-NEXT:                             write<f80, volatile>(%83, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%83, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f80, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%82), read<f80, volatile>(%83))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %182
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %84 a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %85 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%84));
// DEFAULT-NEXT:                         let %86 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(113))
// DEFAULT-NEXT:                             write<f32, volatile>(%86, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%86, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%85), read<f32, volatile>(%86))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %183
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %184
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %87 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %88 b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%87));
// DEFAULT-NEXT:                         let %89 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(53))
// DEFAULT-NEXT:                             write<f16, volatile>(%89, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%89, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%88), read<f16, volatile>(%89))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %185
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %90 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %91 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%90));
// DEFAULT-NEXT:                         let %92 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%92, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%92, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%91), read<f32, volatile>(%92))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %186
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %93 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %94 b: volatile f64 [storage=automatic] = read<f64, volatile>(%93);
// DEFAULT-NEXT:                         let %95 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%95, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%95, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%94), read<f64, volatile>(%95))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %187
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %96 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %97 b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f64, volatile>(%96));
// DEFAULT-NEXT:                         let %98 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(53))
// DEFAULT-NEXT:                             write<f128, volatile>(%98, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%98, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%97), read<f128, volatile>(%98))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %188
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %99 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %100 b: volatile f64 [storage=automatic] = read<f64, volatile>(%99);
// DEFAULT-NEXT:                         let %101 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%101, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%101, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%100), read<f64, volatile>(%101))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %189
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %102 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %103 b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f64, volatile>(%102));
// DEFAULT-NEXT:                         let %104 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(53))
// DEFAULT-NEXT:                             write<f80, volatile>(%104, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%104, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%103), read<f80, volatile>(%104))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %190
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %105 a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %106 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%105));
// DEFAULT-NEXT:                         let %107 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%107, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%107, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%106), read<f32, volatile>(%107))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %191
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %192
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %108 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %109 b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%108));
// DEFAULT-NEXT:                         let %110 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(64))
// DEFAULT-NEXT:                             write<f16, volatile>(%110, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%110, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%109), read<f16, volatile>(%110))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %193
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %111 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %112 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%111));
// DEFAULT-NEXT:                         let %113 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(64))
// DEFAULT-NEXT:                             write<f32, volatile>(%113, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%113, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%112), read<f32, volatile>(%113))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %194
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %114 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %115 b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%114));
// DEFAULT-NEXT:                         let %116 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(64))
// DEFAULT-NEXT:                             write<f64, volatile>(%116, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%116, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%115), read<f64, volatile>(%116))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %195
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %117 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %118 b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f80, volatile>(%117));
// DEFAULT-NEXT:                         let %119 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(64))
// DEFAULT-NEXT:                             write<f128, volatile>(%119, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%119, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%118), read<f128, volatile>(%119))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %196
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %120 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %121 b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%120));
// DEFAULT-NEXT:                         let %122 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(64))
// DEFAULT-NEXT:                             write<f64, volatile>(%122, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%122, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%121), read<f64, volatile>(%122))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %197
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %123 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %124 b: volatile f80 [storage=automatic] = read<f80, volatile>(%123);
// DEFAULT-NEXT:                         let %125 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(64))
// DEFAULT-NEXT:                             write<f80, volatile>(%125, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%125, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19)));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%124), read<f80, volatile>(%125))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %198
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %126 a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %127 b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%126));
// DEFAULT-NEXT:                         let %128 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(64))
// DEFAULT-NEXT:                             write<f32, volatile>(%128, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%128, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%127), read<f32, volatile>(%128))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %199
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %200
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %129 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %130 b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f32, volatile>(%129));
// DEFAULT-NEXT:                         let %131 expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(24))
// DEFAULT-NEXT:                             write<f16, volatile>(%131, int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%131, add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%130), read<f16, volatile>(%131))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %201
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %132 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %133 b: volatile f32 [storage=automatic] = read<f32, volatile>(%132);
// DEFAULT-NEXT:                         let %134 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%134, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%134, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%133), read<f32, volatile>(%134))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %202
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %135 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %136 b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%135));
// DEFAULT-NEXT:                         let %137 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%137, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%137, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%136), read<f64, volatile>(%137))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %203
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %138 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %139 b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f32, volatile>(%138));
// DEFAULT-NEXT:                         let %140 expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(24))
// DEFAULT-NEXT:                             write<f128, volatile>(%140, int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%140, add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%139), read<f128, volatile>(%140))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %204
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %141 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %142 b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%141));
// DEFAULT-NEXT:                         let %143 expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%143, int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%143, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%142), read<f64, volatile>(%143))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %205
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %144 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %145 b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f32, volatile>(%144));
// DEFAULT-NEXT:                         let %146 expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(24))
// DEFAULT-NEXT:                             write<f80, volatile>(%146, int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%146, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%145), read<f80, volatile>(%146))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %206
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %147 a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %148 b: volatile f32 [storage=automatic] = read<f32, volatile>(%147);
// DEFAULT-NEXT:                         let %149 expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%149, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%149, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%148), read<f32, volatile>(%149))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
