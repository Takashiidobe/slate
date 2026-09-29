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
// DEFAULT-NEXT:     fn %[[VALUE_fmod:[0-9]+]] @fmod(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmodf:[0-9]+]] @fmodf(%[[VALUE2:[0-9]+]] <unnamed>: f32, %[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmodl:[0-9]+]] @fmodl(%[[VALUE4:[0-9]+]] <unnamed>: f80, %[[VALUE5:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainder:[0-9]+]] @remainder(%[[VALUE6:[0-9]+]] <unnamed>: f64, %[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainderf:[0-9]+]] @remainderf(%[[VALUE8:[0-9]+]] <unnamed>: f32, %[[VALUE9:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainderl:[0-9]+]] @remainderl(%[[VALUE10:[0-9]+]] <unnamed>: f80, %[[VALUE11:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_drem:[0-9]+]] @drem(%[[VALUE12:[0-9]+]] <unnamed>: f64, %[[VALUE13:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_dremf:[0-9]+]] @dremf(%[[VALUE14:[0-9]+]] <unnamed>: f32, %[[VALUE15:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_dreml:[0-9]+]] @dreml(%[[VALUE16:[0-9]+]] <unnamed>: f80, %[[VALUE17:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_2:[0-9]+]] x: f32, %[[VALUE_y_2:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1l:[0-9]+]] @test1l(%[[VALUE_x_3:[0-9]+]] x: f80, %[[VALUE_y_3:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmodl]], read<f80>(%[[VALUE_x_3]]), read<f80>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_4:[0-9]+]] x: f64, %[[VALUE_y_4:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_remainder]], read<f64>(%[[VALUE_x_4]]), read<f64>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_5:[0-9]+]] x: f32, %[[VALUE_y_5:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_remainderf]], read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2l:[0-9]+]] @test2l(%[[VALUE_x_6:[0-9]+]] x: f80, %[[VALUE_y_6:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_remainderl]], read<f80>(%[[VALUE_x_6]]), read<f80>(%[[VALUE_y_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_7:[0-9]+]] x: f64, %[[VALUE_y_7:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_drem]], read<f64>(%[[VALUE_x_7]]), read<f64>(%[[VALUE_y_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_8:[0-9]+]] x: f32, %[[VALUE_y_8:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_dremf]], read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3l:[0-9]+]] @test3l(%[[VALUE_x_9:[0-9]+]] x: f80, %[[VALUE_y_9:[0-9]+]] y: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_dreml]], read<f80>(%[[VALUE_x_9]]), read<f80>(%[[VALUE_y_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
