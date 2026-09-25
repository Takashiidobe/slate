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

extern void abort(void);
extern void exit(int);

int main(void) {
  f += f;
  if (fetestexcept(FE_INVALID))
    abort();
  exit(0);
}




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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %1 f: volatile f32 [storage=static] = call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%6))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @fetestexcept(%5 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8: f32 [synthetic] = read<f32, volatile>(%1);
// DEFAULT-NEXT:         let %9: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%8), read<f32, volatile>(%1));
// DEFAULT-NEXT:         write<f32, volatile>(%1, read<f32>(%9));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
