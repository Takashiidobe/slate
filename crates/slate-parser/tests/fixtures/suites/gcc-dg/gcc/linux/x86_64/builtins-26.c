/* Copyright (C) 2003 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 28th June 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double trunc(double);
extern double floor(double);
extern double ceil(double);

extern float truncf(float);
extern float floorf(float);
extern float ceilf(float);

extern long double truncl(long double);
extern long double floorl(long double);
extern long double ceill(long double);

void test(double x)
{
  if (trunc (trunc (x)) != trunc (x))
    link_error ();
  if (trunc (floor (x)) != floor (x))
    link_error ();
  if (trunc (ceil (x)) != ceil (x))
    link_error ();

  if (floor (trunc (x)) != trunc (x))
    link_error ();
  if (floor (floor (x)) != floor (x))
    link_error ();
  if (floor (ceil (x)) != ceil (x))
    link_error ();

  if (ceil (trunc (x)) != trunc (x))
    link_error ();
  if (ceil (floor (x)) != floor (x))
    link_error ();
  if (ceil (ceil (x)) != ceil (x))
    link_error ();
}

void testf(float x)
{
  if (truncf (truncf (x)) != truncf (x))
    link_error ();
  if (truncf (floorf (x)) != floorf (x))
    link_error ();
  if (truncf (ceilf (x)) != ceilf (x))
    link_error ();

  if (floorf (truncf (x)) != truncf (x))
    link_error ();
  if (floorf (floorf (x)) != floorf (x))
    link_error ();
  if (floorf (ceilf (x)) != ceilf (x))
    link_error ();

  if (ceilf (truncf (x)) != truncf (x))
    link_error ();
  if (ceilf (floorf (x)) != floorf (x))
    link_error ();
  if (ceilf (ceilf (x)) != ceilf (x))
    link_error ();
}

void testl(long double x)
{
  if (truncl (truncl (x)) != truncl (x))
    link_error ();
  if (truncl (floorl (x)) != floorl (x))
    link_error ();
  if (truncl (ceill (x)) != ceill (x))
    link_error ();

  if (floorl (truncl (x)) != truncl (x))
    link_error ();
  if (floorl (floorl (x)) != floorl (x))
    link_error ();
  if (floorl (ceill (x)) != ceill (x))
    link_error ();

  if (ceill (truncl (x)) != truncl (x))
    link_error ();
  if (ceill (floorl (x)) != floorl (x))
    link_error ();
  if (ceill (ceill (x)) != ceill (x))
    link_error ();
}


int main()
{
  test (3.2);
  testf (3.2f);
  testl (3.2l);
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
// DEFAULT-NEXT:     fn %[[VALUE_trunc:[0-9]+]] @trunc(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floor:[0-9]+]] @floor(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceil:[0-9]+]] @ceil(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncf:[0-9]+]] @truncf(%[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floorf:[0-9]+]] @floorf(%[[VALUE4:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceilf:[0-9]+]] @ceilf(%[[VALUE5:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncl:[0-9]+]] @truncl(%[[VALUE6:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floorl:[0-9]+]] @floorl(%[[VALUE7:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceill:[0-9]+]] @ceill(%[[VALUE8:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_x:[0-9]+]] x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf(%[[VALUE_x_2:[0-9]+]] x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl(%[[VALUE_x_3:[0-9]+]] x: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%[[VALUE_test]], const<f64>(3.2));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%[[VALUE_testf]], const<f32>(3.2));
// DEFAULT-NEXT:         call<void, signature=fn(f80) -> void>(%[[VALUE_testl]], const<f80>(3.20000000000000000004));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
