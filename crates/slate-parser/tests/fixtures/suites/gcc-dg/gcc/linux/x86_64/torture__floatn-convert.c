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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b:[0-9]+]] b: volatile f16 [storage=automatic] = read<f16, volatile>(%[[VALUE_a]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(11))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4)));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b]]), read<f16, volatile>(%[[VALUE_expected]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_2:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b_2:[0-9]+]] b: volatile f32 [storage=automatic] = float_widen<f32, reason=explicit>(read<f16, volatile>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_2:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(11))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_2]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_2]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f32, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_2]]), read<f32, volatile>(%[[VALUE_expected_2]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_3:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b_3:[0-9]+]] b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f16, volatile>(%[[VALUE_a_3]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_3:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(11))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_3]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_3]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_3]]), read<f64, volatile>(%[[VALUE_expected_3]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_4:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b_4:[0-9]+]] b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f16, volatile>(%[[VALUE_a_4]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_4:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(11))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_4]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_4]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_4]]), read<f128, volatile>(%[[VALUE_expected_4]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_5:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b_5:[0-9]+]] b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f16, volatile>(%[[VALUE_a_5]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_5:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(11))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_5]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_5]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_5]]), read<f64, volatile>(%[[VALUE_expected_5]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_6:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b_6:[0-9]+]] b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f16, volatile>(%[[VALUE_a_6]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_6:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(11))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_6]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_6]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_6]]), read<f80, volatile>(%[[VALUE_expected_6]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_7:[0-9]+]] a: volatile f16 [storage=automatic] = add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f16>(9.7656E-4));
// DEFAULT-NEXT:                         let %[[VALUE_b_7:[0-9]+]] b: volatile f32 [storage=automatic] = float_widen<f32, reason=explicit>(read<f16, volatile>(%[[VALUE_a_7]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_7:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(11))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_7]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_7]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f32, reason=explicit>(const<f16>(9.7656E-4))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_7]]), read<f32, volatile>(%[[VALUE_expected_7]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_8:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_8:[0-9]+]] b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f32, volatile>(%[[VALUE_a_8]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_8:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(24))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_8]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_8]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b_8]]), read<f16, volatile>(%[[VALUE_expected_8]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_9:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_9:[0-9]+]] b: volatile f32 [storage=automatic] = read<f32, volatile>(%[[VALUE_a_9]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_9:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_9]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_9]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_9]]), read<f32, volatile>(%[[VALUE_expected_9]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_10:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_10:[0-9]+]] b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%[[VALUE_a_10]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_10:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_10]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_10]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_10]]), read<f64, volatile>(%[[VALUE_expected_10]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_11:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_11:[0-9]+]] b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f32, volatile>(%[[VALUE_a_11]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_11:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(24))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_11]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_11]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_11]]), read<f128, volatile>(%[[VALUE_expected_11]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_12:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_12:[0-9]+]] b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%[[VALUE_a_12]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_12:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_12]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_12]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_12]]), read<f64, volatile>(%[[VALUE_expected_12]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_13:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_13:[0-9]+]] b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f32, volatile>(%[[VALUE_a_13]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_13:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(24))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_13]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_13]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_13]]), read<f80, volatile>(%[[VALUE_expected_13]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_14:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_14:[0-9]+]] b: volatile f32 [storage=automatic] = read<f32, volatile>(%[[VALUE_a_14]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_14:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_14]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_14]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_14]]), read<f32, volatile>(%[[VALUE_expected_14]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_15:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_15:[0-9]+]] b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%[[VALUE_a_15]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_15:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(53))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_15]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_15]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b_15]]), read<f16, volatile>(%[[VALUE_expected_15]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_16:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_16:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%[[VALUE_a_16]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_16:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_16]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_16]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_16]]), read<f32, volatile>(%[[VALUE_expected_16]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_17:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_17:[0-9]+]] b: volatile f64 [storage=automatic] = read<f64, volatile>(%[[VALUE_a_17]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_17:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_17]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_17]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_17]]), read<f64, volatile>(%[[VALUE_expected_17]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_18:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_18:[0-9]+]] b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f64, volatile>(%[[VALUE_a_18]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_18:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(53))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_18]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_18]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_18]]), read<f128, volatile>(%[[VALUE_expected_18]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_19:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_19:[0-9]+]] b: volatile f64 [storage=automatic] = read<f64, volatile>(%[[VALUE_a_19]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_19:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_19]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_19]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_19]]), read<f64, volatile>(%[[VALUE_expected_19]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_20:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_20:[0-9]+]] b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f64, volatile>(%[[VALUE_a_20]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_20:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(53))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_20]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_20]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_20]]), read<f80, volatile>(%[[VALUE_expected_20]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_21:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_21:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%[[VALUE_a_21]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_21:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_21]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_21]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_21]]), read<f32, volatile>(%[[VALUE_expected_21]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_22:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_22:[0-9]+]] b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%[[VALUE_a_22]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_22:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(113))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_22]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_22]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b_22]]), read<f16, volatile>(%[[VALUE_expected_22]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_23:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_23:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%[[VALUE_a_23]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_23:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(113))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_23]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_23]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_23]]), read<f32, volatile>(%[[VALUE_expected_23]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_24:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_24:[0-9]+]] b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%[[VALUE_a_24]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_24:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(113))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_24]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_24]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_24]]), read<f64, volatile>(%[[VALUE_expected_24]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_25:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_25:[0-9]+]] b: volatile f128 [storage=automatic] = read<f128, volatile>(%[[VALUE_a_25]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_25:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(113))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_25]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_25]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34)));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_25]]), read<f128, volatile>(%[[VALUE_expected_25]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_26:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_26:[0-9]+]] b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%[[VALUE_a_26]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_26:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(113))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_26]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_26]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_26]]), read<f64, volatile>(%[[VALUE_expected_26]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_27:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_27:[0-9]+]] b: volatile f80 [storage=automatic] = float_narrow<f80, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%[[VALUE_a_27]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_27:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(113))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_27]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_27]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f80, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_27]]), read<f80, volatile>(%[[VALUE_expected_27]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_28:[0-9]+]] a: volatile f128 [storage=automatic] = add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f128>(1.92592994438723585305597794258492732E-34));
// DEFAULT-NEXT:                         let %[[VALUE_b_28:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f128, volatile>(%[[VALUE_a_28]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_28:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(113))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_28]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_28]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f128>(1.92592994438723585305597794258492732E-34))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_28]]), read<f32, volatile>(%[[VALUE_expected_28]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_29:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_29:[0-9]+]] b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%[[VALUE_a_29]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_29:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(53))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_29]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_29]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b_29]]), read<f16, volatile>(%[[VALUE_expected_29]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_30:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_30:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%[[VALUE_a_30]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_30:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_30]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_30]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_30]]), read<f32, volatile>(%[[VALUE_expected_30]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_31:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_31:[0-9]+]] b: volatile f64 [storage=automatic] = read<f64, volatile>(%[[VALUE_a_31]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_31:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_31]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_31]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_31]]), read<f64, volatile>(%[[VALUE_expected_31]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_32:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_32:[0-9]+]] b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f64, volatile>(%[[VALUE_a_32]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_32:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(53))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_32]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_32]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_32]]), read<f128, volatile>(%[[VALUE_expected_32]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_33:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_33:[0-9]+]] b: volatile f64 [storage=automatic] = read<f64, volatile>(%[[VALUE_a_33]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_33:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(53))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_33]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_33]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16)));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_33]]), read<f64, volatile>(%[[VALUE_expected_33]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_34:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_34:[0-9]+]] b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f64, volatile>(%[[VALUE_a_34]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_34:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(53))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_34]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_34]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_34]]), read<f80, volatile>(%[[VALUE_expected_34]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_35:[0-9]+]] a: volatile f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f64>(2.220446049250313e-16));
// DEFAULT-NEXT:                         let %[[VALUE_b_35:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f64, volatile>(%[[VALUE_a_35]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_35:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(53))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_35]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_35]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(2.220446049250313e-16))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_35]]), read<f32, volatile>(%[[VALUE_expected_35]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_36:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_36:[0-9]+]] b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%[[VALUE_a_36]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_36:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(64))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_36]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_36]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b_36]]), read<f16, volatile>(%[[VALUE_expected_36]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_37:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_37:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%[[VALUE_a_37]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_37:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(64))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_37]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_37]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_37]]), read<f32, volatile>(%[[VALUE_expected_37]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_38:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_38:[0-9]+]] b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%[[VALUE_a_38]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_38:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(64))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_38]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_38]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_38]]), read<f64, volatile>(%[[VALUE_expected_38]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_39:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_39:[0-9]+]] b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f80, volatile>(%[[VALUE_a_39]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_39:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(64))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_39]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_39]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_39]]), read<f128, volatile>(%[[VALUE_expected_39]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_40:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_40:[0-9]+]] b: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%[[VALUE_a_40]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_40:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(64))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_40]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_40]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_40]]), read<f64, volatile>(%[[VALUE_expected_40]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_41:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_41:[0-9]+]] b: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_a_41]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_41:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(64))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_41]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_41]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19)));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_41]]), read<f80, volatile>(%[[VALUE_expected_41]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_42:[0-9]+]] a: volatile f80 [storage=automatic] = add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:                         let %[[VALUE_b_42:[0-9]+]] b: volatile f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f80, volatile>(%[[VALUE_a_42]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_42:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(64))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_42]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_42]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.08420217248550443401E-19))));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_42]]), read<f32, volatile>(%[[VALUE_expected_42]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_43:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_43:[0-9]+]] b: volatile f16 [storage=automatic] = float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(read<f32, volatile>(%[[VALUE_a_43]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_43:[0-9]+]] expected: volatile f16 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(11), const<i32>(24))
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_43]], int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f16, volatile>(%[[VALUE_expected_43]], add<f16, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f16, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f16, exceptions=observable>(read<f16, volatile>(%[[VALUE_b_43]]), read<f16, volatile>(%[[VALUE_expected_43]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_44:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_44:[0-9]+]] b: volatile f32 [storage=automatic] = read<f32, volatile>(%[[VALUE_a_44]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_44:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_44]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_44]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_44]]), read<f32, volatile>(%[[VALUE_expected_44]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_45:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_45:[0-9]+]] b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%[[VALUE_a_45]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_45:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_45]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_45]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_45]]), read<f64, volatile>(%[[VALUE_expected_45]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_46:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_46:[0-9]+]] b: volatile f128 [storage=automatic] = float_widen<f128, reason=explicit>(read<f32, volatile>(%[[VALUE_a_46]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_46:[0-9]+]] expected: volatile f128 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(113), const<i32>(24))
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_46]], int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f128, volatile>(%[[VALUE_expected_46]], add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f128, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f128, exceptions=observable>(read<f128, volatile>(%[[VALUE_b_46]]), read<f128, volatile>(%[[VALUE_expected_46]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_47:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_47:[0-9]+]] b: volatile f64 [storage=automatic] = float_widen<f64, reason=explicit>(read<f32, volatile>(%[[VALUE_a_47]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_47:[0-9]+]] expected: volatile f64 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(53), const<i32>(24))
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_47]], int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f64, volatile>(%[[VALUE_expected_47]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f64, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_b_47]]), read<f64, volatile>(%[[VALUE_expected_47]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_48:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_48:[0-9]+]] b: volatile f80 [storage=automatic] = float_widen<f80, reason=explicit>(read<f32, volatile>(%[[VALUE_a_48]]));
// DEFAULT-NEXT:                         let %[[VALUE_expected_48:[0-9]+]] expected: volatile f80 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(64), const<i32>(24))
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_48]], int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f80, volatile>(%[[VALUE_expected_48]], add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), float_widen<f80, reason=explicit>(const<f32>(1.1920929e-7))));
// DEFAULT-NEXT:                         if ne<f80, exceptions=observable>(read<f80, volatile>(%[[VALUE_b_48]]), read<f80, volatile>(%[[VALUE_expected_48]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_a_49:[0-9]+]] a: volatile f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7));
// DEFAULT-NEXT:                         let %[[VALUE_b_49:[0-9]+]] b: volatile f32 [storage=automatic] = read<f32, volatile>(%[[VALUE_a_49]]);
// DEFAULT-NEXT:                         let %[[VALUE_expected_49:[0-9]+]] expected: volatile f32 [storage=automatic];
// DEFAULT-NEXT:                         if lt<i32>(const<i32>(24), const<i32>(24))
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_49]], int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<f32, volatile>(%[[VALUE_expected_49]], add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), const<f32>(1.1920929e-7)));
// DEFAULT-NEXT:                         if ne<f32, exceptions=observable>(read<f32, volatile>(%[[VALUE_b_49]]), read<f32, volatile>(%[[VALUE_expected_49]]))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
