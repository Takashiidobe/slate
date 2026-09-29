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
// DEFAULT-NEXT:     fn %[[VALUE_log:[0-9]+]] @log(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrt:[0-9]+]] @sqrt(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE3:[0-9]+]] <unnamed>: f64, %[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logf:[0-9]+]] @logf(%[[VALUE5:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expf:[0-9]+]] @expf(%[[VALUE6:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtf:[0-9]+]] @sqrtf(%[[VALUE7:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powf:[0-9]+]] @powf(%[[VALUE8:[0-9]+]] <unnamed>: f32, %[[VALUE9:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logl:[0-9]+]] @logl(%[[VALUE10:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expl:[0-9]+]] @expl(%[[VALUE11:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtl:[0-9]+]] @sqrtl(%[[VALUE12:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powl:[0-9]+]] @powl(%[[VALUE13:[0-9]+]] <unnamed>: f80, %[[VALUE14:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_y_2:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_2]]), read<f64>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_3:[0-9]+]] x: f64, %[[VALUE_y_3:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_x_3]])), read<f64>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_4:[0-9]+]] x: f64, %[[VALUE_y_4:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_x_4]])), read<f64>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_5:[0-9]+]] x: f64, %[[VALUE_y_5:[0-9]+]] y: f64, %[[VALUE_z:[0-9]+]] z: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_5]]), read<f64>(%[[VALUE_y_5]])), read<f64>(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_6:[0-9]+]] x: f32, %[[VALUE_y_6:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_logf]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_7:[0-9]+]] x: f32, %[[VALUE_y_7:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], read<f32>(%[[VALUE_x_7]]), read<f32>(%[[VALUE_y_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_8:[0-9]+]] x: f32, %[[VALUE_y_8:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_x_8]])), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4f:[0-9]+]] @test4f(%[[VALUE_x_9:[0-9]+]] x: f32, %[[VALUE_y_9:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], read<f32>(%[[VALUE_x_9]])), read<f32>(%[[VALUE_y_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5f:[0-9]+]] @test5f(%[[VALUE_x_10:[0-9]+]] x: f32, %[[VALUE_y_10:[0-9]+]] y: f32, %[[VALUE_z_2:[0-9]+]] z: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])), read<f32>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1l:[0-9]+]] @test1l(%[[VALUE_x_11:[0-9]+]] x: f80, %[[VALUE_y_11:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_logl]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], read<f80>(%[[VALUE_x_11]]), read<f80>(%[[VALUE_y_11]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2l:[0-9]+]] @test2l(%[[VALUE_x_12:[0-9]+]] x: f80, %[[VALUE_y_12:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], read<f80>(%[[VALUE_x_12]]), read<f80>(%[[VALUE_y_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3l:[0-9]+]] @test3l(%[[VALUE_x_13:[0-9]+]] x: f80, %[[VALUE_y_13:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_x_13]])), read<f80>(%[[VALUE_y_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4l:[0-9]+]] @test4l(%[[VALUE_x_14:[0-9]+]] x: f80, %[[VALUE_y_14:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], read<f80>(%[[VALUE_x_14]])), read<f80>(%[[VALUE_y_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5l:[0-9]+]] @test5l(%[[VALUE_x_15:[0-9]+]] x: f80, %[[VALUE_y_15:[0-9]+]] y: f80, %[[VALUE_z_3:[0-9]+]] z: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], read<f80>(%[[VALUE_x_15]]), read<f80>(%[[VALUE_y_15]])), read<f80>(%[[VALUE_z_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
