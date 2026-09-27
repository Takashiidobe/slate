/* Copyright (C) 2003,2007 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 5th April 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double exp(double);
extern double sqrt(double);
extern double cbrt(double);
extern double pow(double,double);

void test(double x, double y, double z)
{
  if (sqrt(x)*sqrt(x) != x)
    link_error ();

  if (sqrt(x)*sqrt(y) != sqrt(x*y))
    link_error ();

  if (exp(x)*exp(y) != exp(x+y))
    link_error ();

  if (pow(x,y)*pow(z,y) != pow(z*x,y))
    link_error ();

  if (pow(x,y)*pow(x,z) != pow(x,y+z))
    link_error ();

  if (x/exp(y) != x*exp(-y))
    link_error ();

  if (x/pow(y,z) != x*pow(y,-z))
    link_error ();

  if (x/sqrt(y/z) != x*sqrt(z/y))
    link_error ();

  if (x/cbrt(y/z) != x*cbrt(z/y))
    link_error ();
}

int main()
{
  test (2.0, 3.0, 4.0);
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
// DEFAULT-NEXT:     fn %1 @exp(%10 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sqrt(%11 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @cbrt(%12 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @pow(%13 <unnamed>: f64, %14 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @test(%6 x: f64, %7 y: f64, %8 z: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%6)), call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%6))), read<f64>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%6)), call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%7))), call<f64, signature=fn(f64) -> f64>(%2, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), read<f64>(%7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%6)), call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%7))), call<f64, signature=fn(f64) -> f64>(%1, add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), read<f64>(%7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%6), read<f64>(%7)), call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%8), read<f64>(%7))), call<f64, signature=fn(f64, f64) -> f64>(%4, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%8), read<f64>(%6)), read<f64>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%6), read<f64>(%7)), call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%6), read<f64>(%8))), call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%6), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%7), read<f64>(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%7))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64) -> f64>(%1, neg<f64>(read<f64>(%7)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%7), read<f64>(%8))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64, f64) -> f64>(%4, read<f64>(%7), neg<f64>(read<f64>(%8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64) -> f64>(%2, div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%7), read<f64>(%8)))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64) -> f64>(%2, div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%8), read<f64>(%7)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64) -> f64>(%3, div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%7), read<f64>(%8)))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), call<f64, signature=fn(f64) -> f64>(%3, div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%8), read<f64>(%7)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64) -> void>(%5, const<f64>(2.0), const<f64>(3.0), const<f64>(4.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
