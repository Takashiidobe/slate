/* Tests for _FloatN / _FloatNx types: test usual arithmetic
   conversions.  */
/* { dg-do compile } */
/* { dg-options "" } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float32x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float64 } */
/* { dg-require-effective-target float32x } */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

int i;

#define TEST(VAR, TYPE1, TYPE2, RESTYPE)			\
  do								\
    {								\
      typedef __typeof__ ((TYPE1) 0 + (TYPE2) 1) restype;	\
      typedef __typeof__ (i ? (TYPE1) 0 : (TYPE2) 1) restype2;	\
      typedef RESTYPE exptype;					\
      extern restype VAR;					\
      extern restype2 VAR;					\
      extern exptype VAR;					\
    }								\
  while (0)

void
f (void)
{
  TEST (v1, float, double, double);
#if DBL_MANT_DIG > FLT32_MANT_DIG
  TEST (v2, double, _Float32, double);
#endif
#if DBL_MANT_DIG <= FLT64_MANT_DIG
  TEST (v3, double, _Float64, _Float64);
#endif
#if DBL_MANT_DIG >= FLT32X_MANT_DIG
  TEST (v4, double, _Float32x, double);
#endif
#if FLT_MANT_DIG <= FLT32_MANT_DIG
  TEST (v5, float, _Float32, _Float32);
#endif
#if FLT32X_MANT_DIG <= FLT64_MANT_DIG
  TEST (v6, _Float32x, _Float64, _Float64);
#endif
  TEST (v7, _Float32, _Float64, _Float64);
  TEST (v8, _Float32, _Float32x, _Float32x);
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
// DEFAULT-NEXT:     type @type0 restype = f64;
// DEFAULT-NEXT:     type @type1 restype2 = f64;
// DEFAULT-NEXT:     type @type2 exptype = f64;
// DEFAULT-NEXT:     type @type3 restype = f64;
// DEFAULT-NEXT:     type @type4 restype2 = f64;
// DEFAULT-NEXT:     type @type5 exptype = f64;
// DEFAULT-NEXT:     type @type6 restype = f64;
// DEFAULT-NEXT:     type @type7 restype2 = f64;
// DEFAULT-NEXT:     type @type8 exptype = f64;
// DEFAULT-NEXT:     type @type9 restype = f64;
// DEFAULT-NEXT:     type @type10 restype2 = f64;
// DEFAULT-NEXT:     type @type11 exptype = f64;
// DEFAULT-NEXT:     type @type12 restype = f32;
// DEFAULT-NEXT:     type @type13 restype2 = f32;
// DEFAULT-NEXT:     type @type14 exptype = f32;
// DEFAULT-NEXT:     type @type15 restype = f64;
// DEFAULT-NEXT:     type @type16 restype2 = f64;
// DEFAULT-NEXT:     type @type17 exptype = f64;
// DEFAULT-NEXT:     type @type18 restype = f64;
// DEFAULT-NEXT:     type @type19 restype2 = f64;
// DEFAULT-NEXT:     type @type20 exptype = f64;
// DEFAULT-NEXT:     type @type21 restype = f64;
// DEFAULT-NEXT:     type @type22 restype2 = f64;
// DEFAULT-NEXT:     type @type23 exptype = f64;
// DEFAULT-NEXT:     global %0 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 v1: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %9 v2: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %13 v3: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %17 v4: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %21 v5: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %25 v6: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %29 v7: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %33 v8: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %34
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %35
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %36
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %37
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %38
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %39
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %40
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %41
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
