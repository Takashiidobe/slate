/* Copyright (C) 2004 Free Software Foundation.

   Check that constant folding of signbit, signbitf and signbitl math
   functions doesn't break anything and produces the expected results.

   Written by Roger Sayle, 28th January 2004.  */

/* { dg-do run } */
/* { dg-options "-O2" } */

extern void abort(void);

extern int signbit(double);
extern int signbitf(float);
extern int signbitl(long double);

int test (double x)
{
  return signbit(x);
}

int testf (float x)
{
  return signbitf(x);
}

int testl (long double x)
{
  return signbitl(x);
}


int main()
{
  if (test (0.0) != 0)
    abort ();
  if (test (1.0) != 0)
    abort ();
  if (test (-2.0) == 0)
    abort ();

  if (testf (0.0f) != 0)
    abort ();
  if (testf (1.0f) != 0)
    abort ();
  if (testf (-2.0f) == 0)
    abort ();

  if (testl (0.0l) != 0)
    abort ();
  if (testl (1.0l) != 0)
    abort ();
  if (testl (-2.0l) == 0)
    abort ();

  return 0;
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @signbit(%11 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @signbitf(%12 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @signbitl(%13 <unnamed>: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @test(%5 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f64) -> i32>(%1, read<f64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @testf(%7 x: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f32) -> i32>(%2, read<f32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @testl(%9 x: f80) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(f80) -> i32>(%3, read<f80>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%4, const<f64>(0.0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%4, const<f64>(1.0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f64) -> i32>(%4, neg<f64>(const<f64>(2.0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%6, const<f32>(0.0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%6, const<f32>(1.0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32) -> i32>(%6, neg<f32>(const<f32>(2.0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f80) -> i32>(%8, const<f80>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f80) -> i32>(%8, const<f80>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f80) -> i32>(%8, neg<f80>(const<f80>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
