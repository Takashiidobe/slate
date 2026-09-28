/* Copyright (C) 2004 Free Software Foundation.

   Check that log10, log10f, log10l, log2, log2f and log2l
   built-in functions compile.

   Written by Uros Bizjak, 11th February 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double log10(double);
extern double log2(double);
extern double log1p(double);
extern float log10f(float);
extern float log2f(float);
extern float log1pf(float);
extern long double log10l(long double);
extern long double log2l(long double);
extern long double log1pl(long double);


double test1(double x)
{
  return log10(x);
}

double test2(double x)
{
  return log2(x);
}

double test3(double x)
{
  return log1p(x);
}

float test1f(float x)
{
  return log10f(x);
}

float test2f(float x)
{
  return log2f(x);
}

float test3f(float x)
{
  return log1pf(x);
}

long double test1l(long double x)
{
  return log10l(x);
}

long double test2l(long double x)
{
  return log2l(x);
}

long double test3l(long double x)
{
  return log1pl(x);
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
// DEFAULT-NEXT:     fn %0 @log10(%27 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @log2(%28 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @log1p(%29 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @log10f(%30 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @log2f(%31 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @log1pf(%32 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @log10l(%33 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %7 @log2l(%34 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %8 @log1pl(%35 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %9 @test1(%10 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test2(%12 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test3(%14 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test1f(%16 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%3, read<f32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test2f(%18 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test3f(%20 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test1l(%22 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%6, read<f80>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test2l(%24 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test3l(%26 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
