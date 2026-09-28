/* Copyright (C) 2002  Free Software Foundation.

   Verify that built-in math function constant folding doesn't
   cause any problems for the compiler.

   Written by Roger Sayle, 16th August 2002.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double atan (double);
extern float atanf (float);
extern long double atanl (long double);
extern double exp (double);
extern float expf (float);
extern long double expl (long double);
extern double fabs (double);
extern float fabsf (float);
extern long double fabsl (long double);
extern double log (double);
extern float logf (float);
extern long double logl (long double);
extern double pow (double, double);
extern float powf (float, float);
extern long double powl (long double, long double);
extern double sqrt (double);
extern float sqrtf (float);
extern long double sqrtl (long double);
extern double tan (double);
extern float tanf (float);
extern long double tanl (long double);

double test1(double x)
{
  return log(exp(x));
}

double test2(double x)
{
  return exp(log(x));
}

double test3(double x)
{
  return sqrt(exp(x));
}

double test4(double x)
{
  return log(sqrt(x));
}

double test5(double x, double y)
{
  return sqrt(x)*sqrt(y);
}

double test6(double x, double y)
{
  return exp(x)*exp(y);
}

double test7(double x, double y)
{
  return x/exp(y);
}

double test8(double x)
{
  return fabs(sqrt(x));
}

double test9(double x)
{
  return fabs(exp(x));
}

double test10(double x)
{
  return tan(atan(x));
}

double test11(double x)
{
  return fabs(fabs(x));
}

double test12(double x)
{
  return fabs(atan(x));
}

double test13(double x)
{
  return fabs(pow(2.0,x));
}

float test1f(float x)
{
  return logf(expf(x));
}

float test2f(float x)
{
  return expf(logf(x));
}

float test3f(float x)
{
  return sqrtf(expf(x));
}

float test4f(float x)
{
  return logf(sqrtf(x));
}

float test5f(float x, float y)
{
  return sqrtf(x)*sqrtf(y);
}

float test6f(float x, float y)
{
  return expf(x)*expf(y);
}

float test7f(float x, float y)
{
  return x/expf(y);
}

float test8f(float x)
{
  return fabsf(sqrtf(x));
}

float test9f(float x)
{
  return fabsf(expf(x));
}

float test10f(float x)
{
  return tanf(atanf(x));
}

float test11f(float x)
{
  return fabsf(fabsf(x));
}

float test12f(float x)
{
  return fabsf(atanf(x));
}

float test13f(float x)
{
  return fabsf(powf(2.0f,x));
}

long double test1l(long double x)
{
  return logl(expl(x));
}

long double test2l(long double x)
{
  return expl(logl(x));
}

long double test3l(long double x)
{
  return sqrtl(expl(x));
}

long double test4l(long double x)
{
  return logl(sqrtl(x));
}

long double test5l(long double x, long double y)
{
  return sqrtl(x)*sqrtl(y);
}

long double test6l(long double x, long double y)
{
  return expl(x)*expl(y);
}

long double test7l(long double x, long double y)
{
  return x/expl(y);
}

long double test8l(long double x)
{
  return fabsl(sqrtl(x));
}

long double test9l(long double x)
{
  return fabsl(expl(x));
}

long double test10l(long double x)
{
  return tanl(atanl(x));
}

long double test11l(long double x)
{
  return fabsl(fabsl(x));
}

long double test12l(long double x)
{
  return fabsl(atanl(x));
}

long double test13l(long double x)
{
  return fabsl(powl(2.0l,x));
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
// DEFAULT-NEXT:     fn %0 @atan(%108 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @atanf(%109 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @atanl(%110 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %3 @exp(%111 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @expf(%112 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @expl(%113 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @fabs(%114 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %7 @fabsf(%115 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %8 @fabsl(%116 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %9 @log(%117 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %10 @logf(%118 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @logl(%119 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %12 @pow(%120 <unnamed>: f64, %121 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %13 @powf(%122 <unnamed>: f32, %123 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @powl(%124 <unnamed>: f80, %125 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %15 @sqrt(%126 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @sqrtf(%127 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @sqrtl(%128 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %18 @tan(%129 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %19 @tanf(%130 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @tanl(%131 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %21 @test1(%22 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%9, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%22)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test2(%24 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%9, read<f64>(%24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test3(%26 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%15, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%26)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test4(%28 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%9, call<f64, signature=fn(f64) -> f64>(%15, read<f64>(%28)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test5(%30 x: f64, %31 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%15, read<f64>(%30)), call<f64, signature=fn(f64) -> f64>(%15, read<f64>(%31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @test6(%33 x: f64, %34 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%33)), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%34)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @test7(%36 x: f64, %37 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%36), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%37)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @test8(%39 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%6, call<f64, signature=fn(f64) -> f64>(%15, read<f64>(%39)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test9(%41 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%6, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%41)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @test10(%43 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%18, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%43)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @test11(%45 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%6, call<f64, signature=fn(f64) -> f64>(%6, read<f64>(%45)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @test12(%47 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%6, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%47)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @test13(%49 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%6, call<f64, signature=fn(f64, f64) -> f64>(%12, const<f64>(2.0), read<f64>(%49)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @test1f(%51 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%10, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%51)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @test2f(%53 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%4, call<f32, signature=fn(f32) -> f32>(%10, read<f32>(%53)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @test3f(%55 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%16, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%55)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @test4f(%57 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%10, call<f32, signature=fn(f32) -> f32>(%16, read<f32>(%57)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @test5f(%59 x: f32, %60 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(call<f32, signature=fn(f32) -> f32>(%16, read<f32>(%59)), call<f32, signature=fn(f32) -> f32>(%16, read<f32>(%60)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @test6f(%62 x: f32, %63 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%62)), call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%63)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @test7f(%65 x: f32, %66 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%65), call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%66)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %67 @test8f(%68 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%7, call<f32, signature=fn(f32) -> f32>(%16, read<f32>(%68)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @test9f(%70 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%7, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%70)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @test10f(%72 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%19, call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%72)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %73 @test11f(%74 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%7, call<f32, signature=fn(f32) -> f32>(%7, read<f32>(%74)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @test12f(%76 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%7, call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%76)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @test13f(%78 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%7, call<f32, signature=fn(f32, f32) -> f32>(%13, const<f32>(2.0), read<f32>(%78)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %79 @test1l(%80 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%11, call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%80)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @test2l(%82 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%5, call<f80, signature=fn(f80) -> f80>(%11, read<f80>(%82)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @test3l(%84 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%17, call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%84)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @test4l(%86 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%11, call<f80, signature=fn(f80) -> f80>(%17, read<f80>(%86)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @test5l(%88 x: f80, %89 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(call<f80, signature=fn(f80) -> f80>(%17, read<f80>(%88)), call<f80, signature=fn(f80) -> f80>(%17, read<f80>(%89)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @test6l(%91 x: f80, %92 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%91)), call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%92)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %93 @test7l(%94 x: f80, %95 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%94), call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%95)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @test8l(%97 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%17, read<f80>(%97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @test9l(%99 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%99)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @test10l(%101 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%20, call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%101)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @test11l(%103 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%103)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @test12l(%105 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%105)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @test13l(%107 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80, f80) -> f80>(%14, const<f80>(2), read<f80>(%107)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
