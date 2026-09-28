/* Test NAN macro.  Runtime exceptions test, to verify NaN is quiet
   not signaling.  */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options ieee } */

#include <fenv.h>
#include <float.h>

/* This should be defined if and only if quiet NaNs are supported for
   type float.  If the testsuite gains effective-target support for
   targets not supporting NaNs, or not supporting them for all types,
   this test should only be run for targets supporting quiet NaNs for
   float.  */
#ifndef NAN
#error "NAN undefined"
#endif

volatile float f = NAN;

extern void abort (void);
extern void exit (int);

int
main (void)
{
  f += f;
  if (fetestexcept (FE_INVALID))
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 f: volatile f32 [storage=static] = call<f32, signature=fn(ptr<const i8>) -> f32>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%9))) [linkage=external];
// DEFAULT-NEXT:     fn %1 @fetestexcept(%6 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @__builtin_nanf(%7 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11: f32 [synthetic] = read<f32, volatile>(%2);
// DEFAULT-NEXT:         let %12: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%11), read<f32, volatile>(%2));
// DEFAULT-NEXT:         write<f32, volatile>(%2, read<f32>(%12));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
