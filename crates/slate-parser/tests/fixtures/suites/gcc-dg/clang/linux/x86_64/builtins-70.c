/* Copyright (C) 2021 Free Software Foundation.

   Check that constant folding of built-in fmod functions doesn't
   break anything and produces the expected results.

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double fmod(double,double);
extern float fmodf(float,float);
extern long double fmodl(long double,long double);

int main()
{
  if (fmod (6.5, 2.3) < 1.8999 || fmod (6.5, 2.3) > 1.9001)
    link_error ();
  if (fmod (-6.5, 2.3) < -1.9001 || fmod (-6.5, 2.3) > -1.8999)
    link_error ();
  if (fmod (6.5, -2.3) < 1.8999 || fmod (6.5, -2.3) > 1.9001)
    link_error ();
  if (fmod (-6.5, -2.3) < -1.9001 || fmod (-6.5, -2.3) > -1.8999)
    link_error ();

  if (fmodf (6.5f, 2.3f) < 1.8999f || fmodf (6.5f, 2.3f) > 1.9001f)
    link_error ();
  if (fmodf (-6.5f, 2.3f) < -1.9001f || fmodf (-6.5f, 2.3f) > -1.8999f)
    link_error ();
  if (fmodf (6.5f, -2.3f) < 1.8999f || fmodf (6.5f, -2.3f) > 1.9001f)
    link_error ();
  if (fmodf (-6.5f, -2.3f) < -1.9001f || fmodf (-6.5f, -2.3f) > -1.8999f)
    link_error ();

  if (fmodl (6.5l, 2.3l) < 1.8999l || fmod (6.5l, 2.3l) > 1.9001l)
    link_error ();
  if (fmodl (-6.5l, 2.3l) < -1.9001l || fmod (-6.5l, 2.3l) > -1.8999l)
    link_error ();
  if (fmodl (6.5l, -2.3l) < 1.8999l || fmod (6.5l, -2.3l) > 1.9001l)
    link_error ();
  if (fmodl (-6.5l, -2.3l) < -1.9001l || fmod (-6.5l, -2.3l) > -1.8999l)
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
// DEFAULT-NEXT:     fn %1 @fmod(%5 <unnamed>: f64, %6 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @fmodf(%7 <unnamed>: f32, %8 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @fmodl(%9 <unnamed>: f80, %10 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, const<f64>(6.5), const<f64>(2.3)), const<f64>(1.8999))
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, const<f64>(6.5), const<f64>(2.3)), const<f64>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, neg<f64>(const<f64>(6.5)), const<f64>(2.3)), neg<f64>(const<f64>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, neg<f64>(const<f64>(6.5)), const<f64>(2.3)), neg<f64>(const<f64>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, const<f64>(6.5), neg<f64>(const<f64>(2.3))), const<f64>(1.8999))
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, const<f64>(6.5), neg<f64>(const<f64>(2.3))), const<f64>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, neg<f64>(const<f64>(6.5)), neg<f64>(const<f64>(2.3))), neg<f64>(const<f64>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%1, neg<f64>(const<f64>(6.5)), neg<f64>(const<f64>(2.3))), neg<f64>(const<f64>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(6.5), const<f32>(2.3)), const<f32>(1.8999))
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(6.5), const<f32>(2.3)), const<f32>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, neg<f32>(const<f32>(6.5)), const<f32>(2.3)), neg<f32>(const<f32>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, neg<f32>(const<f32>(6.5)), const<f32>(2.3)), neg<f32>(const<f32>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(6.5), neg<f32>(const<f32>(2.3))), const<f32>(1.8999))
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(6.5), neg<f32>(const<f32>(2.3))), const<f32>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, neg<f32>(const<f32>(6.5)), neg<f32>(const<f32>(2.3))), neg<f32>(const<f32>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, neg<f32>(const<f32>(6.5)), neg<f32>(const<f32>(2.3))), neg<f32>(const<f32>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%3, const<f80>(6.5), const<f80>(2.29999999999999999996)), const<f80>(1.89990000000000000004))
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%1, float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(6.5)), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(2.29999999999999999996)))), const<f80>(1.90010000000000000002)));
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%3, neg<f80>(const<f80>(6.5)), const<f80>(2.29999999999999999996)), neg<f80>(const<f80>(1.90010000000000000002)))
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%1, float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(6.5))), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(2.29999999999999999996)))), neg<f80>(const<f80>(1.89990000000000000004))));
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%3, const<f80>(6.5), neg<f80>(const<f80>(2.29999999999999999996))), const<f80>(1.89990000000000000004))
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%1, float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(6.5)), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(2.29999999999999999996))))), const<f80>(1.90010000000000000002)));
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%3, neg<f80>(const<f80>(6.5)), neg<f80>(const<f80>(2.29999999999999999996))), neg<f80>(const<f80>(1.90010000000000000002)))
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%1, float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(6.5))), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(2.29999999999999999996))))), neg<f80>(const<f80>(1.89990000000000000004))));
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
