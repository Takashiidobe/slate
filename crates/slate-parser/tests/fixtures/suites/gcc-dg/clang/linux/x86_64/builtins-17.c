/* Copyright (C) 2003 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 25th May 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double exp(double);
extern double atan(double);

int main()
{
  if (exp (1.0) < 2.71 || exp (1.0) > 2.72)
    link_error ();
  if (exp (2.0) < 7.38 || exp (2.0) > 7.39)
    link_error ();
  if (exp (-2.0) < 0.13 || exp (-2.0) > 0.14)
    link_error ();
  if (atan (1.0) < 0.78 || atan (1.0) > 0.79)
    link_error ();

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
// DEFAULT-NEXT:     fn %0 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exp(%4 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @atan(%5 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, const<f64>(1.0)), const<f64>(2.71))
// DEFAULT-NEXT:             write<bool>(%6, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%6, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, const<f64>(1.0)), const<f64>(2.72)));
// DEFAULT-NEXT:         if read<bool>(%6)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %7: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, const<f64>(2.0)), const<f64>(7.38))
// DEFAULT-NEXT:             write<bool>(%7, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%7, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, const<f64>(2.0)), const<f64>(7.39)));
// DEFAULT-NEXT:         if read<bool>(%7)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, neg<f64>(const<f64>(2.0))), const<f64>(0.13))
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%1, neg<f64>(const<f64>(2.0))), const<f64>(0.14)));
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%2, const<f64>(1.0)), const<f64>(0.78))
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%2, const<f64>(1.0)), const<f64>(0.79)));
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
