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
// DEFAULT-NEXT:     fn %[[VALUE_rint:[0-9]+]] @rint(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrint:[0-9]+]] @lrint(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrint:[0-9]+]] @llrint(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_floor:[0-9]+]] @floor(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceil:[0-9]+]] @ceil(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_trunc:[0-9]+]] @trunc(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyint:[0-9]+]] @nearbyint(%[[VALUE6:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_rintf:[0-9]+]] @rintf(%[[VALUE7:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrintf:[0-9]+]] @lrintf(%[[VALUE8:[0-9]+]] <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrintf:[0-9]+]] @llrintf(%[[VALUE9:[0-9]+]] <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_floorf:[0-9]+]] @floorf(%[[VALUE10:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceilf:[0-9]+]] @ceilf(%[[VALUE11:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncf:[0-9]+]] @truncf(%[[VALUE12:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyintf:[0-9]+]] @nearbyintf(%[[VALUE13:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_rintl:[0-9]+]] @rintl(%[[VALUE14:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrintl:[0-9]+]] @lrintl(%[[VALUE15:[0-9]+]] <unnamed>: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrintl:[0-9]+]] @llrintl(%[[VALUE16:[0-9]+]] <unnamed>: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_floorl:[0-9]+]] @floorl(%[[VALUE17:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceill:[0-9]+]] @ceill(%[[VALUE18:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncl:[0-9]+]] @truncl(%[[VALUE19:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyintl:[0-9]+]] @nearbyintl(%[[VALUE20:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_rint]], read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11:[0-9]+]] @test11(%[[VALUE_x_2:[0-9]+]] x: f64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f64) -> i64>(%[[VALUE_lrint]], read<f64>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12:[0-9]+]] @test12(%[[VALUE_x_3:[0-9]+]] x: f64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f64) -> i64>(%[[VALUE_llrint]], read<f64>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_4:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_5:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_6:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_x_7:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_nearbyint]], read<f64>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_8:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_rintf]], read<f32>(%[[VALUE_x_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11f:[0-9]+]] @test11f(%[[VALUE_x_9:[0-9]+]] x: f32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f32) -> i64>(%[[VALUE_lrintf]], read<f32>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12f:[0-9]+]] @test12f(%[[VALUE_x_10:[0-9]+]] x: f32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f32) -> i64>(%[[VALUE_llrintf]], read<f32>(%[[VALUE_x_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_11:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_12:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4f:[0-9]+]] @test4f(%[[VALUE_x_13:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5f:[0-9]+]] @test5f(%[[VALUE_x_14:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_nearbyintf]], read<f32>(%[[VALUE_x_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1l:[0-9]+]] @test1l(%[[VALUE_x_15:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_rintl]], read<f80>(%[[VALUE_x_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11l:[0-9]+]] @test11l(%[[VALUE_x_16:[0-9]+]] x: f80) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f80) -> i64>(%[[VALUE_lrintl]], read<f80>(%[[VALUE_x_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12l:[0-9]+]] @test12l(%[[VALUE_x_17:[0-9]+]] x: f80) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i64, signature=fn(f80) -> i64>(%[[VALUE_llrintl]], read<f80>(%[[VALUE_x_17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2l:[0-9]+]] @test2l(%[[VALUE_x_18:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3l:[0-9]+]] @test3l(%[[VALUE_x_19:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4l:[0-9]+]] @test4l(%[[VALUE_x_20:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5l:[0-9]+]] @test5l(%[[VALUE_x_21:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_nearbyintl]], read<f80>(%[[VALUE_x_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
