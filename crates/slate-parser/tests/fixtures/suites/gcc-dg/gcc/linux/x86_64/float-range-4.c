/* PR 23572 : warnings for out of range floating-point constants.  */
/* { dg-do compile } */
/* { dg-options "-Wno-overflow -std=c99" } */
#include <math.h>

#ifndef INFINITY
#define INFINITY (__builtin_inff ())
#endif

void overflow(void)
{
  float f1 = 3.5E+38f;  
  float f2 = -3.5E+38f; 
  float f3 = INFINITY;
  float f4 = -INFINITY;

  double d1 = 1.9E+308; 
  double d2 = -1.9E+308;
  double d3 = INFINITY;
  double d4 = -INFINITY;
}

void underflow(void)
{
  float f11 = 3.3E-10000000000000000000f;
  float f22 = -3.3E-10000000000000000000f;
  float f1 = 3.3E-46f;  
  float f2 = -3.3E-46f; 
  float f3 = 0;
  float f4 = -0;
  float f5 = 0.0;
  float f6 = -0.0;

  double d11 = 3.3E-10000000000000000000;
  double d22 = -3.3E-10000000000000000000;
  double d1 = 1.4E-325; 
  double d2 = -1.4E-325;
  double d3 = 0;
  double d4 = -0;
  double d5 = 0.0;
  double d6 = -0.0;
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     fn %26 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @overflow() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1 f1: f32 [storage=automatic] = const<f32>(inf);
// DEFAULT-NEXT:         let %2 f2: f32 [storage=automatic] = neg<f32>(const<f32>(inf));
// DEFAULT-NEXT:         let %3 f3: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%26);
// DEFAULT-NEXT:         let %4 f4: f32 [storage=automatic] = neg<f32>(call<f32, signature=fn() -> f32>(%26));
// DEFAULT-NEXT:         let %5 d1: f64 [storage=automatic] = const<f64>(inf);
// DEFAULT-NEXT:         let %6 d2: f64 [storage=automatic] = neg<f64>(const<f64>(inf));
// DEFAULT-NEXT:         let %7 d3: f64 [storage=automatic] = float_widen<f64, reason=assign>(call<f32, signature=fn() -> f32>(%26));
// DEFAULT-NEXT:         let %8 d4: f64 [storage=automatic] = float_widen<f64, reason=assign>(neg<f32>(call<f32, signature=fn() -> f32>(%26)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @underflow() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 f11: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %11 f22: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:         let %12 f1: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %13 f2: f32 [storage=automatic] = neg<f32>(const<f32>(0.0));
// DEFAULT-NEXT:         let %14 f3: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %15 f4: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(0)));
// DEFAULT-NEXT:         let %16 f5: f32 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.0));
// DEFAULT-NEXT:         let %17 f6: f32 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         let %18 d11: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %19 d22: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:         let %20 d1: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %21 d2: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:         let %22 d3: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         let %23 d4: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(0)));
// DEFAULT-NEXT:         let %24 d5: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %25 d6: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
