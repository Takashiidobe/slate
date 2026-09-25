/* Test NAN macro.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options ieee } */

#include <float.h>

/* This should be defined if and only if quiet NaNs are supported for
   type float.  If the testsuite gains effective-target support for
   targets not supporting NaNs, or not supporting them for all types,
   this test should be split into versions for targets with and
   without NaNs for float.  */
#ifndef NAN
#error "NAN undefined"
#endif

volatile float f = NAN;

extern void abort(void);
extern void exit(int);

int main(void) {
  (void)_Generic(NAN, float: 0);
  if (!__builtin_isnan(NAN))
    abort();
  if (!__builtin_isnan(f))
    abort();
  if (!__builtin_isnan(f + f))
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
// DEFAULT-NEXT:     global %4 .str4: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %0 f: volatile f32 [storage=static] = call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%4))) [linkage=external];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<f32, volatile>(%0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32, volatile>(%0), read<f32, volatile>(%0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
