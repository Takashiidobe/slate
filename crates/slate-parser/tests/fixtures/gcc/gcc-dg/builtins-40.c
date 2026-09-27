/* Copyright (C) 2004 Free Software Foundation.

   Check that fmod, fmodf, fmodl, drem, dremf, dreml,
   remainder, remainderf and remainderl
   built-in functions compile.

   Written by Uros Bizjak, 5th May 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2" } */

extern double fmod(double, double);
extern float fmodf(float, float);
extern long double fmodl(long double, long double);

extern double remainder(double, double);
extern float remainderf(float, float);
extern long double remainderl(long double, long double);

extern double drem(double, double);
extern float dremf(float, float);
extern long double dreml(long double, long double);


double test1(double x, double y)
{
  return fmod(x, y);
}

float test1f(float x, float y)
{
  return fmodf(x, y);
}

long double test1l(long double x, long double y)
{
  return fmodl(x, y);
}

double test2(double x, double y)
{
  return remainder(x, y);
}

float test2f(float x, float y)
{
  return remainderf(x, y);
}

long double test2l(long double x, long double y)
{
  return remainderl(x, y);
}

double test3(double x, double y)
{
  return drem(x, y);
}

float test3f(float x, float y)
{
  return dremf(x, y);
}

long double test3l(long double x, long double y)
{
  return dreml(x, y);
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
// DEFAULT-NEXT:     fn %0 @fmod(%36 <unnamed>: f64, %37 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @fmodf(%38 <unnamed>: f32, %39 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @fmodl(%40 <unnamed>: f80, %41 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %3 @remainder(%42 <unnamed>: f64, %43 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @remainderf(%44 <unnamed>: f32, %45 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @remainderl(%46 <unnamed>: f80, %47 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @drem(%48 <unnamed>: f64, %49 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @dremf(%50 <unnamed>: f32, %51 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @dreml(%52 <unnamed>: f80, %53 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %9 @test1(%10 x: f64, %11 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%0, read<f64>(%10), read<f64>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test1f(%13 x: f32, %14 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%1, read<f32>(%13), read<f32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test1l(%16 x: f80, %17 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%2, read<f80>(%16), read<f80>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test2(%19 x: f64, %20 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%3, read<f64>(%19), read<f64>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test2f(%22 x: f32, %23 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%4, read<f32>(%22), read<f32>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test2l(%25 x: f80, %26 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%5, read<f80>(%25), read<f80>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test3(%28 x: f64, %29 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%6, read<f64>(%28), read<f64>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test3f(%31 x: f32, %32 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%7, read<f32>(%31), read<f32>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test3l(%34 x: f80, %35 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%8, read<f80>(%34), read<f80>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
