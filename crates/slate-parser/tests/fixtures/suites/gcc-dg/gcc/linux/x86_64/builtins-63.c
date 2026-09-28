/* Copyright (C) 2007 Free Software Foundation.

   Check that isinf, isinff and isinfl built-in functions compile.

   Written by Uros Bizjak, 31st January 2007.  */

/* { dg-do compile } */
/* { dg-options "-O2" } */

extern int isinf(double);
extern int isinff(float);
extern int isinfl(long double);

int test1(double x)
{
  return isinf(x);
}

int test1f(float x)
{
  return isinff(x);
}

int test1l(long double x)
{
  return isinfl(x);
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
// DEFAULT-NEXT:     fn %0 @isinf(%9 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @isinff(%10 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @isinfl(%11 <unnamed>: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @test1(%4 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f64) -> i32>(%0, read<f64>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test1f(%6 x: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f32) -> i32>(%1, read<f32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test1l(%8 x: f80) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f80) -> i32>(%2, read<f80>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
