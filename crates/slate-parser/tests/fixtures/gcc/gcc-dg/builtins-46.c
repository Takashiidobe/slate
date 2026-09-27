/* Copyright (C) 2004 Free Software Foundation.

   Check that rint, rintf, rintl, lrint, lrintf, lrintl,
   llrint, llrintf, llrintl, floor, floorf, floorl,
   ceil, ceilf, ceill, trunc, truncf, truncl,
   nearbyint, nearbyintf and nearbyintl
   built-in functions compile.

   Written by Uros Bizjak, 25th Aug 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double rint(double);
extern long int lrint(double);
extern long long int llrint(double);
extern double floor(double);
extern double ceil(double);
extern double trunc(double);
extern double nearbyint(double);

extern float rintf(float);
extern long int lrintf(float);
extern long long int llrintf(float);
extern float floorf(float);
extern float ceilf(float);
extern float truncf(float);
extern float nearbyintf(float);

extern long double rintl(long double);
extern long int lrintl(long double);
extern long long int llrintl(long double);
extern long double floorl(long double);
extern long double ceill(long double);
extern long double truncl(long double);
extern long double nearbyintl(long double);


double test1(double x)
{
  return rint(x);
}

long int test11(double x)
{
  return lrint(x);
}

long long int test12(double x)
{
  return llrint(x);
}

double test2(double x)
{
  return floor(x);
}

double test3(double x)
{
  return ceil(x);
}

double test4(double x)
{
  return trunc(x);
}

double test5(double x)
{
  return nearbyint(x);
}

float test1f(float x)
{
  return rintf(x);
}

long int test11f(float x)
{
  return lrintf(x);
}

long long int test12f(float x)
{
  return llrintf(x);
}

float test2f(float x)
{
  return floorf(x);
}

float test3f(float x)
{
  return ceilf(x);
}

float test4f(float x)
{
  return truncf(x);
}

float test5f(float x)
{
  return nearbyintf(x);
}

long double test1l(long double x)
{
  return rintl(x);
}

long int test11l(long double x)
{
  return lrintl(x);
}

long long int test12l(long double x)
{
  return llrintl(x);
}

long double test2l(long double x)
{
  return floorl(x);
}

long double test3l(long double x)
{
  return ceill(x);
}

long double test4l(long double x)
{
  return truncl(x);
}

long double test5l(long double x)
{
  return nearbyintl(x);
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
// DEFAULT-NEXT:     fn %0 @rint(%63 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @lrint(%64 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @llrint(%65 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @floor(%66 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @ceil(%67 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %5 @trunc(%68 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @nearbyint(%69 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %7 @rintf(%70 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @lrintf(%71 <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %9 @llrintf(%72 <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %10 @floorf(%73 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %11 @ceilf(%74 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %12 @truncf(%75 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %13 @nearbyintf(%76 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %14 @rintl(%77 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %15 @lrintl(%78 <unnamed>: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @llrintl(%79 <unnamed>: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %17 @floorl(%80 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %18 @ceill(%81 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %19 @truncl(%82 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %20 @nearbyintl(%83 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %21 @test1(%22 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test11(%24 x: f64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f64) -> i64>(%1, read<f64>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test12(%26 x: f64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f64) -> i64>(%2, read<f64>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test2(%28 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test3(%30 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%4, read<f64>(%30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test4(%32 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%5, read<f64>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test5(%34 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%6, read<f64>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @test1f(%36 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%7, read<f32>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @test11f(%38 x: f32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f32) -> i64>(%8, read<f32>(%38));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @test12f(%40 x: f32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f32) -> i64>(%9, read<f32>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test2f(%42 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%10, read<f32>(%42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @test3f(%44 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%11, read<f32>(%44));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @test4f(%46 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%12, read<f32>(%46));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @test5f(%48 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%13, read<f32>(%48));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @test1l(%50 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%14, read<f80>(%50));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @test11l(%52 x: f80) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f80) -> i64>(%15, read<f80>(%52));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @test12l(%54 x: f80) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f80) -> i64>(%16, read<f80>(%54));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @test2l(%56 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%17, read<f80>(%56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @test3l(%58 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%18, read<f80>(%58));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @test4l(%60 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%19, read<f80>(%60));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @test5l(%62 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%20, read<f80>(%62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
