/* Copyright (C) 2003  Free Software Foundation.

   Verify that all the __builtin_cabs? functions are recognized
   by the compiler.  Complex numbers are not supported with the
   gcc.dg default "-pedantic-errors" option, so the dg-options
   overrides this.

   Written by Roger Sayle, 1st June 2003.  */

/* { dg-do compile } */
/* { dg-options "-O -ansi" } */
/* { dg-final { scan-assembler-not "__builtin_" } } */

double test(__complex__ double x)
{
  return __builtin_cabs (x);
}

float testf(__complex__ float x)
{
  return __builtin_cabsf (x);
}

long double testl(__complex__ long double x)
{
  return __builtin_cabsl (x);
}


// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cabs:[0-9]+]] @__builtin_cabs(%[[VALUE0:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_x:[0-9]+]] x: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cabs]], read<complex<f64>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cabsf:[0-9]+]] @__builtin_cabsf(%[[VALUE1:[0-9]+]] <unnamed>: complex<f32>) -> f32 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf(%[[VALUE_x_2:[0-9]+]] x: complex<f32>) -> f32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cabsf]], read<complex<f32>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cabsl:[0-9]+]] @__builtin_cabsl(%[[VALUE2:[0-9]+]] <unnamed>: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl(%[[VALUE_x_3:[0-9]+]] x: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cabsl]], read<complex<f80>>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
