/* Copyright (C) 2004 Free Software Foundation.

   Check that logb, logbf, logbl, ilogb, ilogbf and ilogbl
   built-in functions compile.

   Written by Uros Bizjak, 14th April 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double logb(double);
extern float logbf(float);
extern long double logbl(long double);
extern int ilogb(double);
extern int ilogbf(float);
extern int ilogbl(long double);


double test1(double x)
{
  return logb(x);
}

float test1f(float x)
{
  return logbf(x);
}

long double test1l(long double x)
{
  return logbl(x);
}

int test2(double x)
{
  return ilogb(x);
}

int test2f(float x)
{
  return ilogbf(x);
}

int test2l(long double x)
{
  return ilogbl(x);
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
// DEFAULT-NEXT:     fn %0 @logb(%18 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @logbf(%19 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @logbl(%20 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %3 @ilogb(%21 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @ilogbf(%22 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @ilogbl(%23 <unnamed>: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @test1(%7 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test1f(%9 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test1l(%11 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test2(%13 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f64) -> i32>(%3, read<f64>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test2f(%15 x: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f32) -> i32>(%4, read<f32>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test2l(%17 x: f80) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f80) -> i32>(%5, read<f80>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
