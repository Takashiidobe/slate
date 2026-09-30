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
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmod:[0-9]+]] @fmod(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmodf:[0-9]+]] @fmodf(%[[VALUE2:[0-9]+]] <unnamed>: f32, %[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmodl:[0-9]+]] @fmodl(%[[VALUE4:[0-9]+]] <unnamed>: f80, %[[VALUE5:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], const<f64>(6.5), const<f64>(2.3)), const<f64>(1.8999))
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], const<f64>(6.5), const<f64>(2.3)), const<f64>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], neg<f64>(const<f64>(6.5)), const<f64>(2.3)), neg<f64>(const<f64>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], neg<f64>(const<f64>(6.5)), const<f64>(2.3)), neg<f64>(const<f64>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], const<f64>(6.5), neg<f64>(const<f64>(2.3))), const<f64>(1.8999))
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], const<f64>(6.5), neg<f64>(const<f64>(2.3))), const<f64>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], neg<f64>(const<f64>(6.5)), neg<f64>(const<f64>(2.3))), neg<f64>(const<f64>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], gt<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], neg<f64>(const<f64>(6.5)), neg<f64>(const<f64>(2.3))), neg<f64>(const<f64>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], const<f32>(6.5), const<f32>(2.3)), const<f32>(1.8999))
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], const<f32>(6.5), const<f32>(2.3)), const<f32>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], neg<f32>(const<f32>(6.5)), const<f32>(2.3)), neg<f32>(const<f32>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], neg<f32>(const<f32>(6.5)), const<f32>(2.3)), neg<f32>(const<f32>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], const<f32>(6.5), neg<f32>(const<f32>(2.3))), const<f32>(1.8999))
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], const<f32>(6.5), neg<f32>(const<f32>(2.3))), const<f32>(1.9001)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], neg<f32>(const<f32>(6.5)), neg<f32>(const<f32>(2.3))), neg<f32>(const<f32>(1.9001)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], gt<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], neg<f32>(const<f32>(6.5)), neg<f32>(const<f32>(2.3))), neg<f32>(const<f32>(1.8999))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmodl]], const<f80>(6.5), const<f80>(2.29999999999999999996)), const<f80>(1.89990000000000000004))
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(6.5)), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(2.29999999999999999996)))), const<f80>(1.90010000000000000002)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmodl]], neg<f80>(const<f80>(6.5)), const<f80>(2.29999999999999999996)), neg<f80>(const<f80>(1.90010000000000000002)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(6.5))), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(2.29999999999999999996)))), neg<f80>(const<f80>(1.89990000000000000004))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE15]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmodl]], const<f80>(6.5), neg<f80>(const<f80>(2.29999999999999999996))), const<f80>(1.89990000000000000004))
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f80>(6.5)), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(2.29999999999999999996))))), const<f80>(1.90010000000000000002)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE16]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmodl]], neg<f80>(const<f80>(6.5)), neg<f80>(const<f80>(2.29999999999999999996))), neg<f80>(const<f80>(1.90010000000000000002)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], gt<f80, exceptions=ignore>(float_widen<f80, reason=usual_arith>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(6.5))), float_narrow<f64, reason=arg, rounding=nearest_even, exceptions=ignore>(neg<f80>(const<f80>(2.29999999999999999996))))), neg<f80>(const<f80>(1.89990000000000000004))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE17]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
