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
// DEFAULT-NEXT:     fn %0 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @trunc(%17 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @floor(%18 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @ceil(%19 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @truncf(%20 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %5 @floorf(%21 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @ceilf(%22 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %7 @truncl(%23 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %8 @floorl(%24 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %9 @ceill(%25 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %10 @test(%11 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%1, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%1, call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%1, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%2, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%2, call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%2, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%11))), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @testf(%13 x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%4, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%4, call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%4, call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%5, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%5, call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%5, call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%6, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%6, call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%5, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%6, call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%13))), call<f32, signature=fn(f32) -> f32>(%6, read<f32>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @testl(%15 x: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%7, call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%7, call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%7, call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%8, call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%9, call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%7, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%9, call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%8, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%9, call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%15))), call<f80, signature=fn(f80) -> f80>(%9, read<f80>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%10, const<f64>(3.2));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%12, const<f32>(3.2));
// DEFAULT-NEXT:         call<void, signature=fn(f80) -> void>(%14, const<f80>(3.20000000000000000004));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
