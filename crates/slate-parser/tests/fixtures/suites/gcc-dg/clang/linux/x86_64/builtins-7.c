/* Copyright (C) 2003  Free Software Foundation.

   Verify that built-in math function constant folding of constant
   arguments is correctly performed by the by the compiler.

   Written by Roger Sayle, 30th March 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern double pow (double, double);
extern float powf (float, float);
extern long double powl (long double, long double);
extern double tan (double);
extern float tanf (float);
extern long double tanl (long double);
extern double atan (double);
extern float atanf (float);
extern long double atanl (long double);

extern void link_error(void);

void test(double x)
{
  if (pow (x, 1.0) != x)
    link_error ();
  if (tan (atan (x)) != x)
    link_error ();
}

void testf(float x)
{
  if (powf (x, 1.0f) != x)
    link_error ();
  if (tanf (atanf (x)) != x)
    link_error ();
}

void testl(long double x)
{
  if (powl (x, 1.0l) != x)
    link_error ();
  if (tanl (atanl (x)) != x)
    link_error ();
}

int main()
{
  test (2.0);
  testf (2.0f);
  testl (2.0l);

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
// DEFAULT-NEXT:     fn %0 @pow(%17 <unnamed>: f64, %18 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @powf(%19 <unnamed>: f32, %20 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @powl(%21 <unnamed>: f80, %22 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %3 @tan(%23 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @tanf(%24 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @tanl(%25 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @atan(%26 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @atanf(%27 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @atanl(%28 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %9 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @test(%11 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%0, read<f64>(%11), const<f64>(1.0)), read<f64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%6, read<f64>(%11))), read<f64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @testf(%13 x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%1, read<f32>(%13), const<f32>(1.0)), read<f32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%4, call<f32, signature=fn(f32) -> f32>(%7, read<f32>(%13))), read<f32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @testl(%15 x: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%2, read<f80>(%15), const<f80>(1)), read<f80>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%5, call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15))), read<f80>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%10, const<f64>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%12, const<f32>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f80) -> void>(%14, const<f80>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
