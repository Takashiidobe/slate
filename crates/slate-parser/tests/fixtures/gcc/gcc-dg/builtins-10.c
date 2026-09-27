/* Copyright (C) 2003 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 2nd April 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double exp(double);
extern double log(double);
extern double sqrt(double);
extern double pow(double,double);
extern double fabs(double);

void test(double x)
{
  if (sqrt(pow(x,4.0)) != x*x)
    link_error ();

  if (pow(sqrt(x),4.0) != x*x)
    link_error ();

  if (pow(pow(x,4.0),0.25) != x)
    /* XFAIL.  PR41098.  */;
}

void test2(double x, double y, double z)
{
  if (sqrt(pow(x,y)) != pow(fabs(x),y*0.5))
    link_error ();

  if (log(pow(x,y)) != y*log(x))
    link_error ();

  if (pow(exp(x),y) != exp(x*y))
    link_error ();

  if (pow(sqrt(x),y) != pow(x,y*0.5))
    link_error ();

  if (pow(pow(fabs(x),y),z) != pow(fabs(x),y*z))
    link_error ();
}

int main()
{
  test (2.0);
  test2 (2.0, 3.0, 4.0);
  return 0;
}


// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exp(%13 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @log(%14 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @sqrt(%15 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @pow(%16 <unnamed>: f64, %17 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @fabs(%18 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @test(%7 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%7), const<f64>(4.0))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%7), read<f64>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%7)), const<f64>(4.0)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%7), read<f64>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%7), const<f64>(4.0)), const<f64>(0.25)), read<f64>(%7))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test2(%9 x: f64, %10 y: f64, %11 z: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%9), read<f64>(%10))), call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64) -> f64>(%5, read<f64>(%9)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%10), const<f64>(0.5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%2, call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%9), read<f64>(%10))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%10), call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%9)), read<f64>(%10)), call<f64, signature=fn(f64) -> f64>(%1, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), read<f64>(%10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%9)), read<f64>(%10)), call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%9), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%10), const<f64>(0.5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64) -> f64>(%5, read<f64>(%9)), read<f64>(%10)), read<f64>(%11)), call<f64, signature=fn(f64, f64) -> f64>(%4, call<f64, signature=fn(f64) -> f64>(%5, read<f64>(%9)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%10), read<f64>(%11))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%6, const<f64>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64) -> void>(%8, const<f64>(2.0), const<f64>(3.0), const<f64>(4.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
