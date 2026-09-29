/* Copyright (C) 2004 Free Software Foundation.

   Check that constant folding of copysign, copysignf and copysignl math
   functions doesn't break anything and produces the expected results.

   Written by Roger Sayle, 6th June 2004.  */

/* { dg-do link } */
/* { dg-options "-O2" } */

extern void link_error(void);

extern double copysign(double, double);
extern float copysignf(float, float);
extern long double copysignl(long double, long double);

int main()
{
  if (copysign (2.0, 1.0) != 2.0)
    link_error ();
  if (copysign (2.0, -1.0) != -2.0)
    link_error ();
  if (copysign (-2.0, 1.0) != 2.0)
    link_error ();
  if (copysign (-2.0, -1.0) != -2.0)
    link_error ();

  if (copysign (2.0, 1.0) != 2.0)
    link_error ();
  if (copysign (2.0, -1.0) != -2.0)
    link_error ();
  if (copysign (-2.0, 1.0) != 2.0)
    link_error ();
  if (copysign (-2.0, -1.0) != -2.0)
    link_error ();

  if (copysignf (2.0f, 1.0f) != 2.0f)
    link_error ();
  if (copysignf (2.0f, -1.0f) != -2.0f)
    link_error ();
  if (copysignf (-2.0f, 1.0f) != 2.0f)
    link_error ();
  if (copysignf (-2.0f, -1.0f) != -2.0f)
    link_error ();

  if (copysignl (2.0l, 1.0l) != 2.0l)
    link_error ();
  if (copysignl (2.0l, -1.0l) != -2.0l)
    link_error ();
  if (copysignl (-2.0l, 1.0l) != 2.0l)
    link_error ();
  if (copysignl (-2.0l, -1.0l) != -2.0l)
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
// DEFAULT-NEXT:     fn %[[VALUE_copysign:[0-9]+]] @copysign(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_copysignf:[0-9]+]] @copysignf(%[[VALUE2:[0-9]+]] <unnamed>: f32, %[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_copysignl:[0-9]+]] @copysignl(%[[VALUE4:[0-9]+]] <unnamed>: f80, %[[VALUE5:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], const<f64>(2.0), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], const<f64>(2.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], neg<f64>(const<f64>(2.0)), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], const<f64>(2.0), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], const<f64>(2.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], neg<f64>(const<f64>(2.0)), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], const<f32>(2.0), const<f32>(1.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], const<f32>(2.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], neg<f32>(const<f32>(2.0)), const<f32>(1.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_copysignl]], const<f80>(2), const<f80>(1)), const<f80>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_copysignl]], const<f80>(2), neg<f80>(const<f80>(1))), neg<f80>(const<f80>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_copysignl]], neg<f80>(const<f80>(2)), const<f80>(1)), const<f80>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_copysignl]], neg<f80>(const<f80>(2)), neg<f80>(const<f80>(1))), neg<f80>(const<f80>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
