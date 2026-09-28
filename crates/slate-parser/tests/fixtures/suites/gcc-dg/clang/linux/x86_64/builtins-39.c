/* Copyright (C) 2004 Free Software Foundation.

   Check that asin, asinf, asinl, acos, acosf
   and acosl built-in functions compile.

   Written by Uros Bizjak, 20th April 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double asin(double);
extern double acos(double);
extern float asinf(float);
extern float acosf(float);
extern long double asinl(long double);
extern long double acosl(long double);


double test1(double x)
{
  return asin(x);
}

double test2(double x)
{
  return acos(x);
}

float test1f(float x)
{
  return asinf(x);
}

float test2f(float x)
{
  return acosf(x);
}

long double test1l(long double x)
{
  return asinl(x);
}

long double test2l(long double x)
{
  return acosl(x);
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
// DEFAULT-NEXT:     fn %0 @asin(%18 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @acos(%19 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @asinf(%20 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @acosf(%21 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @asinl(%22 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %5 @acosl(%23 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @test1(%7 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test2(%9 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test1f(%11 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%2, read<f32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test2f(%13 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%3, read<f32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test1l(%15 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%4, read<f80>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test2l(%17 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
