/* Copyright (C) 2004 Free Software Foundation.

   Check that various built-in functions compile.

   Written by Uros Bizjak, 13th February 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double exp10(double);
extern double exp2(double);
extern double pow10(double);
extern double expm1(double);
extern double ldexp(double, int);
extern double scalb(double, double);
extern double scalbn(double, int);
extern double scalbln(double, long);
extern double significand(double);
extern float exp10f(float);
extern float exp2f(float);
extern float pow10f(float);
extern float expm1f(float);
extern float ldexpf(float, int);
extern float scalbf(float, float);
extern float scalbnf(float, int);
extern float scalblnf(float, long);
extern float significandf(float);
extern long double exp10l(long double);
extern long double exp2l(long double);
extern long double pow10l(long double);
extern long double expm1l(long double);
extern long double ldexpl(long double, int);
extern long double scalbl(long double, long double);
extern long double scalbnl(long double, int);
extern long double scalblnl(long double, long);
extern long double significandl(long double);


double test1(double x)
{
  return exp10(x);
}

double test2(double x)
{
  return exp2(x);
}

double test3(double x)
{
  return pow10(x);
}

double test4(double x)
{
  return expm1(x);
}

double test5(double x, int exp)
{
  return ldexp(x, exp);
}

double test6(double x, double exp)
{
  return scalb(x, exp);
}

double test7(double x, int exp)
{
  return scalbn(x, exp);
}

double test8(double x, long exp)
{
  return scalbln(x, exp);
}

double test9(double x)
{
  return significand(x);
}

float test1f(float x)
{
  return exp10f(x);
}

float test2f(float x)
{
  return exp2f(x);
}

float test3f(float x)
{
  return pow10f(x);
}

float test4f(float x)
{
  return expm1f(x);
}

float test5f(float x, int exp)
{
  return ldexpf(x, exp);
}

float test6f(float x, float exp)
{
  return scalbf(x, exp);
}

float test7f(float x, int exp)
{
  return scalbnf(x, exp);
}

float test8f(float x, long exp)
{
  return scalblnf(x, exp);
}

float test9f(float x)
{
  return significandf(x);
}

long double test1l(long double x)
{
  return exp10l(x);
}

long double test2l(long double x)
{
  return exp2l(x);
}

long double test3l(long double x)
{
  return pow10l(x);
}

long double test4l(long double x)
{
  return expm1l(x);
}

long double test5l(long double x, int exp)
{
  return ldexpl(x, exp);
}

long double test6l(long double x, long double exp)
{
  return scalbl(x, exp);
}

long double test7l(long double x, int exp)
{
  return scalbnl(x, exp);
}

long double test8l(long double x, long exp)
{
  return scalblnl(x, exp);
}

long double test9l(long double x)
{
  return significandl(x);
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
// DEFAULT-NEXT:     fn %0 @exp10(%93 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @exp2(%94 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @pow10(%95 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @expm1(%96 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @ldexp(%97 <unnamed>: f64, %98 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @scalb(%99 <unnamed>: f64, %100 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %6 @scalbn(%101 <unnamed>: f64, %102 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @scalbln(%103 <unnamed>: f64, %104 <unnamed>: i64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @significand(%105 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %9 @exp10f(%106 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @exp2f(%107 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @pow10f(%108 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @expm1f(%109 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @ldexpf(%110 <unnamed>: f32, %111 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @scalbf(%112 <unnamed>: f32, %113 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @scalbnf(%114 <unnamed>: f32, %115 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @scalblnf(%116 <unnamed>: f32, %117 <unnamed>: i64) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @significandf(%118 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @exp10l(%119 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %19 @exp2l(%120 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %20 @pow10l(%121 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %21 @expm1l(%122 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %22 @ldexpl(%123 <unnamed>: f80, %124 <unnamed>: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %23 @scalbl(%125 <unnamed>: f80, %126 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %24 @scalbnl(%127 <unnamed>: f80, %128 <unnamed>: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %25 @scalblnl(%129 <unnamed>: f80, %130 <unnamed>: i64) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %26 @significandl(%131 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %27 @test1(%28 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test2(%30 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test3(%32 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test4(%34 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @test5(%36 x: f64, %37 exp: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%4, read<f64>(%36), read<i32>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @test6(%39 x: f64, %40 exp: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%5, read<f64>(%39), read<f64>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test7(%42 x: f64, %43 exp: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%6, read<f64>(%42), read<i32>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @test8(%45 x: f64, %46 exp: i64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i64) -> f64>(%7, read<f64>(%45), read<i64>(%46));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @test9(%48 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%8, read<f64>(%48));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @test1f(%50 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%9, read<f32>(%50));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @test2f(%52 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%10, read<f32>(%52));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @test3f(%54 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%11, read<f32>(%54));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @test4f(%56 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%12, read<f32>(%56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @test5f(%58 x: f32, %59 exp: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%13, read<f32>(%58), read<i32>(%59));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @test6f(%61 x: f32, %62 exp: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%14, read<f32>(%61), read<f32>(%62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @test7f(%64 x: f32, %65 exp: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%15, read<f32>(%64), read<i32>(%65));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @test8f(%67 x: f32, %68 exp: i64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i64) -> f32>(%16, read<f32>(%67), read<i64>(%68));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @test9f(%70 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%17, read<f32>(%70));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @test1l(%72 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%18, read<f80>(%72));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %73 @test2l(%74 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%19, read<f80>(%74));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @test3l(%76 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%20, read<f80>(%76));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @test4l(%78 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%21, read<f80>(%78));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %79 @test5l(%80 x: f80, %81 exp: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%22, read<f80>(%80), read<i32>(%81));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %82 @test6l(%83 x: f80, %84 exp: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%23, read<f80>(%83), read<f80>(%84));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @test7l(%86 x: f80, %87 exp: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%24, read<f80>(%86), read<i32>(%87));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @test8l(%89 x: f80, %90 exp: i64) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i64) -> f80>(%25, read<f80>(%89), read<i64>(%90));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @test9l(%92 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%26, read<f80>(%92));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
