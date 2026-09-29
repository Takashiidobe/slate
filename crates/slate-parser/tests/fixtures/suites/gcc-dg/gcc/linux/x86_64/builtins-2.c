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
// DEFAULT-NEXT:     fn %[[VALUE_atan:[0-9]+]] @atan(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanf:[0-9]+]] @atanf(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanl:[0-9]+]] @atanl(%[[VALUE2:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expf:[0-9]+]] @expf(%[[VALUE4:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expl:[0-9]+]] @expl(%[[VALUE5:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE6:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsf:[0-9]+]] @fabsf(%[[VALUE7:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsl:[0-9]+]] @fabsl(%[[VALUE8:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_log:[0-9]+]] @log(%[[VALUE9:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logf:[0-9]+]] @logf(%[[VALUE10:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logl:[0-9]+]] @logl(%[[VALUE11:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE12:[0-9]+]] <unnamed>: f64, %[[VALUE13:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powf:[0-9]+]] @powf(%[[VALUE14:[0-9]+]] <unnamed>: f32, %[[VALUE15:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powl:[0-9]+]] @powl(%[[VALUE16:[0-9]+]] <unnamed>: f80, %[[VALUE17:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrt:[0-9]+]] @sqrt(%[[VALUE18:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtf:[0-9]+]] @sqrtf(%[[VALUE19:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtl:[0-9]+]] @sqrtl(%[[VALUE20:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tan:[0-9]+]] @tan(%[[VALUE21:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanf:[0-9]+]] @tanf(%[[VALUE22:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanl:[0-9]+]] @tanl(%[[VALUE23:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], read<f64>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_4:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_5:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_x_5]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_x_6:[0-9]+]] x: f64, %[[VALUE_y_2:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_x_6]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_x_7:[0-9]+]] x: f64, %[[VALUE_y_3:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x_7]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_y_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_x_8:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_x_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9:[0-9]+]] @test9(%[[VALUE_x_9:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_x_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10:[0-9]+]] @test10(%[[VALUE_x_10:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_tan]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_atan]], read<f64>(%[[VALUE_x_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11:[0-9]+]] @test11(%[[VALUE_x_11:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_11]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12:[0-9]+]] @test12(%[[VALUE_x_12:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_atan]], read<f64>(%[[VALUE_x_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test13:[0-9]+]] @test13(%[[VALUE_x_13:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], const<f64>(2.0), read<f64>(%[[VALUE_x_13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_14:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_logf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_x_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_15:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_logf]], read<f32>(%[[VALUE_x_15]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_16:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_x_16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4f:[0-9]+]] @test4f(%[[VALUE_x_17:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_logf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], read<f32>(%[[VALUE_x_17]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5f:[0-9]+]] @test5f(%[[VALUE_x_18:[0-9]+]] x: f32, %[[VALUE_y_4:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], read<f32>(%[[VALUE_x_18]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], read<f32>(%[[VALUE_y_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6f:[0-9]+]] @test6f(%[[VALUE_x_19:[0-9]+]] x: f32, %[[VALUE_y_5:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_x_19]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_y_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7f:[0-9]+]] @test7f(%[[VALUE_x_20:[0-9]+]] x: f32, %[[VALUE_y_6:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_x_20]]), call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_y_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8f:[0-9]+]] @test8f(%[[VALUE_x_21:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], read<f32>(%[[VALUE_x_21]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9f:[0-9]+]] @test9f(%[[VALUE_x_22:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_x_22]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10f:[0-9]+]] @test10f(%[[VALUE_x_23:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_tanf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_atanf]], read<f32>(%[[VALUE_x_23]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11f:[0-9]+]] @test11f(%[[VALUE_x_24:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_x_24]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12f:[0-9]+]] @test12f(%[[VALUE_x_25:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_atanf]], read<f32>(%[[VALUE_x_25]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test13f:[0-9]+]] @test13f(%[[VALUE_x_26:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], const<f32>(2.0), read<f32>(%[[VALUE_x_26]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1l:[0-9]+]] @test1l(%[[VALUE_x_27:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_logl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_x_27]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2l:[0-9]+]] @test2l(%[[VALUE_x_28:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_logl]], read<f80>(%[[VALUE_x_28]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3l:[0-9]+]] @test3l(%[[VALUE_x_29:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_x_29]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4l:[0-9]+]] @test4l(%[[VALUE_x_30:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_logl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], read<f80>(%[[VALUE_x_30]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5l:[0-9]+]] @test5l(%[[VALUE_x_31:[0-9]+]] x: f80, %[[VALUE_y_7:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], read<f80>(%[[VALUE_x_31]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], read<f80>(%[[VALUE_y_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6l:[0-9]+]] @test6l(%[[VALUE_x_32:[0-9]+]] x: f80, %[[VALUE_y_8:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_x_32]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_y_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7l:[0-9]+]] @test7l(%[[VALUE_x_33:[0-9]+]] x: f80, %[[VALUE_y_9:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_x_33]]), call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_y_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8l:[0-9]+]] @test8l(%[[VALUE_x_34:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], read<f80>(%[[VALUE_x_34]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9l:[0-9]+]] @test9l(%[[VALUE_x_35:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE_x_35]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10l:[0-9]+]] @test10l(%[[VALUE_x_36:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_tanl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_atanl]], read<f80>(%[[VALUE_x_36]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11l:[0-9]+]] @test11l(%[[VALUE_x_37:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_x_37]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12l:[0-9]+]] @test12l(%[[VALUE_x_38:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_atanl]], read<f80>(%[[VALUE_x_38]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test13l:[0-9]+]] @test13l(%[[VALUE_x_39:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], const<f80>(2), read<f80>(%[[VALUE_x_39]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
