/* Copyright (C) 2003  Free Software Foundation.

   Verify that constant folding comparisons against built-in math functions
   don't cause any problems for the compiler, and produce expected results.

   Written by Roger Sayle, 15th March 2003.  */

/* { dg-do run } */
/* { dg-options "-O2 -ffast-math" } */

#include <float.h>

extern void abort (void);
extern double sqrt (double);

int test1(double x)
{
  return sqrt(x) < -9.0;
}

int test2(double x)
{
  return sqrt(x) > -9.0;
}

int test3(double x)
{
  return sqrt(x) < 9.0;
}

int test4(double x)
{
  return sqrt(x) > 9.0;
}

int test5(double x)
{
  return sqrt(x) < DBL_MAX;
}

int test6(double x)
{
  return sqrt(x) > DBL_MAX;
}

int main()
{
  double x;

  x = 80.0;
  if (test1 (x))
    abort ();
  if (! test2 (x))
    abort ();
  if (! test3 (x))
    abort ();
  if (test4 (x))
    abort ();
  if (! test5 (x))
    abort ();
  if (test6 (x))
    abort ();

  x = 100.0;
  if (test1 (x))
    abort ();
  if (! test2 (x))
    abort ();
  if (test3 (x))
    abort ();
  if (! test4 (x))
    abort ();
  if (! test5 (x))
    abort ();
  if (test6 (x))
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
// DEFAULT-NEXT:     fn %1 @sqrt(%16 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @test1(%3 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%3)), neg<f64>(const<f64>(9.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test2(%5 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%5)), neg<f64>(const<f64>(9.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test3(%7 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%7)), const<f64>(9.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test4(%9 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%9)), const<f64>(9.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test5(%11 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11)), const<f64>(1.7976931348623157e308)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test6(%13 x: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%13)), const<f64>(1.7976931348623157e308)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 x: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%15, const<f64>(80.0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%2, read<f64>(%15)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%4, read<f64>(%15)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%6, read<f64>(%15)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%8, read<f64>(%15)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%10, read<f64>(%15)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%12, read<f64>(%15)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<f64>(%15, const<f64>(100.0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%2, read<f64>(%15)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%4, read<f64>(%15)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%6, read<f64>(%15)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%8, read<f64>(%15)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%10, read<f64>(%15)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%12, read<f64>(%15)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
