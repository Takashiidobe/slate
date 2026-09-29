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
// DEFAULT-NEXT:     fn %[[VALUE_exp10:[0-9]+]] @exp10(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2:[0-9]+]] @exp2(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow10:[0-9]+]] @pow10(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1:[0-9]+]] @expm1(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexp:[0-9]+]] @ldexp(%[[VALUE4:[0-9]+]] <unnamed>: f64, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalb:[0-9]+]] @scalb(%[[VALUE6:[0-9]+]] <unnamed>: f64, %[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbn:[0-9]+]] @scalbn(%[[VALUE8:[0-9]+]] <unnamed>: f64, %[[VALUE9:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbln:[0-9]+]] @scalbln(%[[VALUE10:[0-9]+]] <unnamed>: f64, %[[VALUE11:[0-9]+]] <unnamed>: i64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_significand:[0-9]+]] @significand(%[[VALUE12:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10f:[0-9]+]] @exp10f(%[[VALUE13:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2f:[0-9]+]] @exp2f(%[[VALUE14:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow10f:[0-9]+]] @pow10f(%[[VALUE15:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1f:[0-9]+]] @expm1f(%[[VALUE16:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexpf:[0-9]+]] @ldexpf(%[[VALUE17:[0-9]+]] <unnamed>: f32, %[[VALUE18:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbf:[0-9]+]] @scalbf(%[[VALUE19:[0-9]+]] <unnamed>: f32, %[[VALUE20:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbnf:[0-9]+]] @scalbnf(%[[VALUE21:[0-9]+]] <unnamed>: f32, %[[VALUE22:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalblnf:[0-9]+]] @scalblnf(%[[VALUE23:[0-9]+]] <unnamed>: f32, %[[VALUE24:[0-9]+]] <unnamed>: i64) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_significandf:[0-9]+]] @significandf(%[[VALUE25:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10l:[0-9]+]] @exp10l(%[[VALUE26:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2l:[0-9]+]] @exp2l(%[[VALUE27:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow10l:[0-9]+]] @pow10l(%[[VALUE28:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1l:[0-9]+]] @expm1l(%[[VALUE29:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexpl:[0-9]+]] @ldexpl(%[[VALUE30:[0-9]+]] <unnamed>: f80, %[[VALUE31:[0-9]+]] <unnamed>: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbl:[0-9]+]] @scalbl(%[[VALUE32:[0-9]+]] <unnamed>: f80, %[[VALUE33:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbnl:[0-9]+]] @scalbnl(%[[VALUE34:[0-9]+]] <unnamed>: f80, %[[VALUE35:[0-9]+]] <unnamed>: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalblnl:[0-9]+]] @scalblnl(%[[VALUE36:[0-9]+]] <unnamed>: f80, %[[VALUE37:[0-9]+]] <unnamed>: i64) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_significandl:[0-9]+]] @significandl(%[[VALUE38:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp10]], read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp2]], read<f64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_pow10]], read<f64>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_4:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_expm1]], read<f64>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_5:[0-9]+]] x: f64, %[[VALUE_exp:[0-9]+]] exp: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_ldexp]], read<f64>(%[[VALUE_x_5]]), read<i32>(%[[VALUE_exp]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_x_6:[0-9]+]] x: f64, %[[VALUE_exp_2:[0-9]+]] exp: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_scalb]], read<f64>(%[[VALUE_x_6]]), read<f64>(%[[VALUE_exp_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_x_7:[0-9]+]] x: f64, %[[VALUE_exp_3:[0-9]+]] exp: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_scalbn]], read<f64>(%[[VALUE_x_7]]), read<i32>(%[[VALUE_exp_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_x_8:[0-9]+]] x: f64, %[[VALUE_exp_4:[0-9]+]] exp: i64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i64) -> f64>(%[[VALUE_scalbln]], read<f64>(%[[VALUE_x_8]]), read<i64>(%[[VALUE_exp_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9:[0-9]+]] @test9(%[[VALUE_x_9:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_significand]], read<f64>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_10:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_exp10f]], read<f32>(%[[VALUE_x_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_11:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_exp2f]], read<f32>(%[[VALUE_x_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_12:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_pow10f]], read<f32>(%[[VALUE_x_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4f:[0-9]+]] @test4f(%[[VALUE_x_13:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_expm1f]], read<f32>(%[[VALUE_x_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5f:[0-9]+]] @test5f(%[[VALUE_x_14:[0-9]+]] x: f32, %[[VALUE_exp_5:[0-9]+]] exp: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE_ldexpf]], read<f32>(%[[VALUE_x_14]]), read<i32>(%[[VALUE_exp_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6f:[0-9]+]] @test6f(%[[VALUE_x_15:[0-9]+]] x: f32, %[[VALUE_exp_6:[0-9]+]] exp: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_scalbf]], read<f32>(%[[VALUE_x_15]]), read<f32>(%[[VALUE_exp_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7f:[0-9]+]] @test7f(%[[VALUE_x_16:[0-9]+]] x: f32, %[[VALUE_exp_7:[0-9]+]] exp: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE_scalbnf]], read<f32>(%[[VALUE_x_16]]), read<i32>(%[[VALUE_exp_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8f:[0-9]+]] @test8f(%[[VALUE_x_17:[0-9]+]] x: f32, %[[VALUE_exp_8:[0-9]+]] exp: i64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i64) -> f32>(%[[VALUE_scalblnf]], read<f32>(%[[VALUE_x_17]]), read<i64>(%[[VALUE_exp_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9f:[0-9]+]] @test9f(%[[VALUE_x_18:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_significandf]], read<f32>(%[[VALUE_x_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1l:[0-9]+]] @test1l(%[[VALUE_x_19:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_exp10l]], read<f80>(%[[VALUE_x_19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2l:[0-9]+]] @test2l(%[[VALUE_x_20:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_exp2l]], read<f80>(%[[VALUE_x_20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3l:[0-9]+]] @test3l(%[[VALUE_x_21:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_pow10l]], read<f80>(%[[VALUE_x_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4l:[0-9]+]] @test4l(%[[VALUE_x_22:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_expm1l]], read<f80>(%[[VALUE_x_22]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5l:[0-9]+]] @test5l(%[[VALUE_x_23:[0-9]+]] x: f80, %[[VALUE_exp_9:[0-9]+]] exp: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE_ldexpl]], read<f80>(%[[VALUE_x_23]]), read<i32>(%[[VALUE_exp_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6l:[0-9]+]] @test6l(%[[VALUE_x_24:[0-9]+]] x: f80, %[[VALUE_exp_10:[0-9]+]] exp: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_scalbl]], read<f80>(%[[VALUE_x_24]]), read<f80>(%[[VALUE_exp_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7l:[0-9]+]] @test7l(%[[VALUE_x_25:[0-9]+]] x: f80, %[[VALUE_exp_11:[0-9]+]] exp: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE_scalbnl]], read<f80>(%[[VALUE_x_25]]), read<i32>(%[[VALUE_exp_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8l:[0-9]+]] @test8l(%[[VALUE_x_26:[0-9]+]] x: f80, %[[VALUE_exp_12:[0-9]+]] exp: i64) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i64) -> f80>(%[[VALUE_scalblnl]], read<f80>(%[[VALUE_x_26]]), read<i64>(%[[VALUE_exp_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9l:[0-9]+]] @test9l(%[[VALUE_x_27:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_significandl]], read<f80>(%[[VALUE_x_27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
