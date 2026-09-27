/* Copyright (C) 2003 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 29th July 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double pow(double,double);

void test(double x)
{
  if (pow(x,2.0) != x*x)
    link_error ();

  if (x*pow(x,2.0) != pow(x,3.0))
    link_error ();

  if (pow(x,2.0)*x != pow(x,3.0))
    link_error ();

  if (pow(x,3.0) != x*x*x)
    link_error ();

  if (pow(x,2.0)*x != x*x*x)
    link_error ();

  if (x*pow(x,2.0) != x*x*x)
    link_error ();

  if (pow(x,3.0)/x != pow(x,2.0))
    link_error ();

  if (pow(x,3.0)/x != x*x)
    link_error ();
}

int main()
{
  test (2.0);
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
// DEFAULT-NEXT:     fn %1 @pow(%5 <unnamed>: f64, %6 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @test(%3 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(2.0)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), read<f64>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(2.0))), call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(2.0)), read<f64>(%3)), call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(3.0)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), read<f64>(%3)), read<f64>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(2.0)), read<f64>(%3)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), read<f64>(%3)), read<f64>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(2.0))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), read<f64>(%3)), read<f64>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(3.0)), read<f64>(%3)), call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%1, read<f64>(%3), const<f64>(3.0)), read<f64>(%3)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%3), read<f64>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%2, const<f64>(2.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
