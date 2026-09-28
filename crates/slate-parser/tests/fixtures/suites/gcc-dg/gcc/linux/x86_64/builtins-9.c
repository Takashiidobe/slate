/* Copyright (C) 2003 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything.

   Written by Roger Sayle, 2nd April 2003.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double log(double);
extern double exp(double);
extern double sqrt(double);
extern double pow(double,double);

extern float logf(float);
extern float expf(float);
extern float sqrtf(float);
extern float powf(float,float);

extern long double logl(long double);
extern long double expl(long double);
extern long double sqrtl(long double);
extern long double powl(long double,long double);


double test1(double x, double y)
{
  return log(pow(x,y));
}

double test2(double x, double y)
{
  return sqrt(pow(x,y));
}

double test3(double x, double y)
{
  return pow(exp(x),y);
}

double test4(double x, double y)
{
  return pow(sqrt(x),y);
}

double test5(double x, double y, double z)
{
  return pow(pow(x,y),z);
}


float test1f(float x, float y)
{
  return logf(powf(x,y));
}

float test2f(float x, float y)
{
  return sqrtf(powf(x,y));
}

float test3f(float x, float y)
{
  return powf(expf(x),y);
}

float test4f(float x, float y)
{
  return powf(sqrtf(x),y);
}

float test5f(float x, float y, float z)
{
  return powf(powf(x,y),z);
}


long double test1l(long double x, long double y)
{
  return logl(powl(x,y));
}

long double test2l(long double x, long double y)
{
  return sqrtl(powl(x,y));
}

long double test3l(long double x, long double y)
{
  return powl(expl(x),y);
}

long double test4l(long double x, long double y)
{
  return powl(sqrtl(x),y);
}

long double test5l(long double x, long double y, long double z)
{
  return powl(powl(x,y),z);
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
// DEFAULT-NEXT:     fn %0 @log(%60 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @exp(%61 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sqrt(%62 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @pow(%63 <unnamed>: f64, %64 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @logf(%65 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @expf(%66 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @sqrtf(%67 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @powf(%68 <unnamed>: f32, %69 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @logl(%70 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %9 @expl(%71 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %10 @sqrtl(%72 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %11 @powl(%73 <unnamed>: f80, %74 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %12 @test1(%13 x: f64, %14 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, call<f64, signature=fn(f64, f64) -> f64>(%3, read<f64>(%13), read<f64>(%14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test2(%16 x: f64, %17 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%2, call<f64, signature=fn(f64, f64) -> f64>(%3, read<f64>(%16), read<f64>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test3(%19 x: f64, %20 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%19)), read<f64>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test4(%22 x: f64, %23 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%22)), read<f64>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test5(%25 x: f64, %26 y: f64, %27 z: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%3, call<f64, signature=fn(f64, f64) -> f64>(%3, read<f64>(%25), read<f64>(%26)), read<f64>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test1f(%29 x: f32, %30 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%4, call<f32, signature=fn(f32, f32) -> f32>(%7, read<f32>(%29), read<f32>(%30)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test2f(%32 x: f32, %33 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%6, call<f32, signature=fn(f32, f32) -> f32>(%7, read<f32>(%32), read<f32>(%33)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test3f(%35 x: f32, %36 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%7, call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%35)), read<f32>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @test4f(%38 x: f32, %39 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%7, call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%38)), read<f32>(%39));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test5f(%41 x: f32, %42 y: f32, %43 z: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%7, call<f32, signature=fn(f32, f32) -> f32>(%7, read<f32>(%41), read<f32>(%42)), read<f32>(%43));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @test1l(%45 x: f80, %46 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80, f80) -> f80>(%11, read<f80>(%45), read<f80>(%46)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @test2l(%48 x: f80, %49 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%10, call<f80, signature=fn(f80, f80) -> f80>(%11, read<f80>(%48), read<f80>(%49)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @test3l(%51 x: f80, %52 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%11, call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%51)), read<f80>(%52));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @test4l(%54 x: f80, %55 y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%11, call<f80, signature=fn(f80) -> f80>(%10, read<f80>(%54)), read<f80>(%55));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @test5l(%57 x: f80, %58 y: f80, %59 z: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%11, call<f80, signature=fn(f80, f80) -> f80>(%11, read<f80>(%57), read<f80>(%58)), read<f80>(%59));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
