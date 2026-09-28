/* Copyright (C) 2003  Free Software Foundation.

   Check that cabs of a non-complex argument is converted into fabs.

   Written by Roger Sayle, 1st June 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

double cabs (__complex__ double);
float cabsf (__complex__ float);
long double cabsl (__complex__ long double);
double fabs (double);
float fabsf (float);
long double fabsl (long double);

void link_error (void);

void test (double x)
{
  if (cabs (x) != fabs (x))
    link_error ();
}

void testf (float x)
{
  if (cabsf (x) != fabsf (x))
    link_error ();
}

void testl (long double x)
{
  if (cabsl (x) != fabsl (x))
    link_error ();
}

int main ()
{
  test (1.0);
  testf (1.0f);
  testl (1.0l);
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
// DEFAULT-NEXT:     fn %0 @cabs(%14 <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %1 @cabsf(%15 <unnamed>: complex<f32>) -> f32 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %2 @cabsl(%16 <unnamed>: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %3 @fabs(%17 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @fabsf(%18 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %5 @fabsl(%19 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %6 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @test(%8 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%0, real_to_complex<complex<f64>, reason=arg>(read<f64>(%8))), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @testf(%10 x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%1, real_to_complex<complex<f32>, reason=arg>(read<f32>(%10))), call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @testl(%12 x: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%2, real_to_complex<complex<f80>, reason=arg>(read<f80>(%12))), call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%7, const<f64>(1.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%9, const<f32>(1.0));
// DEFAULT-NEXT:         call<void, signature=fn(f80) -> void>(%11, const<f80>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
