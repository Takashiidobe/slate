/* Copyright (C) 2003  Free Software Foundation.

   Verify that built-in math function constant folding doesn't
   cause any problems for the compiler.

   Written by Roger Sayle, 7th June 2003.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double fabs (double);
extern float fabsf (float);
extern long double fabsl (long double);
extern double sqrt (double);
extern float sqrtf (float);
extern long double sqrtl (long double);
extern double exp (double);
extern float expf (float);
extern long double expl (long double);

double test1(double x)
{
  return fabs(x*x);
}

double test2(double x)
{
  return fabs(sqrt(x)+2.0);
}

double test3(double x)
{
  return fabs(3.0*exp(x));
}

float test1f(float x)
{
  return fabsf(x*x);
}

float test2f(float x)
{
  return fabsf(sqrtf(x)+2.0f);
}

float test3f(float x)
{
  return fabsf(3.0f*expf(x));
}

long double test1l(long double x)
{
  return fabsl(x*x);
}

long double test2l(long double x)
{
  return fabsl(sqrtl(x)+2.0l);
}

long double test3l(long double x)
{
  return fabsl(3.0l*expl(x));
}


// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @fabs(%27 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @fabsf(%28 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @fabsl(%29 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @sqrt(%30 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @sqrtf(%31 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @sqrtl(%32 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @exp(%33 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @expf(%34 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @expl(%35 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %9 @test1(%10 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%10), read<f64>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test2(%12 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%12)), const<f64>(2.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test3(%14 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(3.0), call<f64, signature=fn(f64) -> f64>(%6, read<f64>(%14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test1f(%16 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%16), read<f32>(%16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test2f(%18 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%18)), const<f32>(2.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test3f(%20 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(const<f32>(3.0), call<f32, signature=fn(f32) -> f32>(%7, read<f32>(%20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test1l(%22 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%2, mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%22), read<f80>(%22)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test2l(%24 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%2, add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%24)), const<f80>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test3l(%26 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%2, mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(const<f80>(3), call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%26))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
