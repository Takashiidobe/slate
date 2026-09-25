/* Test _Float16 NaNs.  */
/* { dg-do run } */
/* { dg-options "-fsignaling-nans" } */
/* { dg-add-options float16 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float16_runtime } */
/* { dg-require-effective-target fenv_exceptions } */

#define WIDTH 16
#define EXT   0
/* Tests for _FloatN / _FloatNx types: compile and execution tests for
   NaNs.  Before including this file, define WIDTH as the value N;
   define EXT to 1 for _FloatNx and 0 for _FloatN.  */

#define CONCATX(X, Y)       X##Y
#define CONCAT(X, Y)        CONCATX(X, Y)
#define CONCAT3(X, Y, Z)    CONCAT(CONCAT(X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT(CONCAT(CONCAT(W, X), Y), Z)

#if EXT
#define TYPE   CONCAT3(_Float, WIDTH, x)
#define CST(C) CONCAT4(C, f, WIDTH, x)
#define FN(F)  CONCAT4(F, f, WIDTH, x)
#else
#define TYPE   CONCAT(_Float, WIDTH)
#define CST(C) CONCAT3(C, f, WIDTH)
#define FN(F)  CONCAT3(F, f, WIDTH)
#endif

#include <fenv.h>

extern void exit(int);
extern void abort(void);

volatile TYPE nan_cst  = FN(__builtin_nan)("");
volatile TYPE nans_cst = FN(__builtin_nans)("");

int
main(void) {
  volatile TYPE r;
  r = nan_cst + nan_cst;
  if (fetestexcept(FE_INVALID))
    abort();
  r = nans_cst + nans_cst;
  if (!fetestexcept(FE_INVALID))
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 nan_cst: volatile f16 [storage=static] = call<f16, signature=fn(ptr<const i8>) -> f16>(__builtin_nanf16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%9))) [linkage=external];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 nans_cst: volatile f16 [storage=static] = call<f16, signature=fn(ptr<const i8>) -> f16>(__builtin_nansf16, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%10))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @fetestexcept(%7 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 r: volatile f16 [storage=automatic];
// DEFAULT-NEXT:         write<f16, volatile>(%6, add<f16, rounding=nearest_even, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%3)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%6, add<f16, rounding=nearest_even, exceptions=ignore>(read<f16, volatile>(%4), read<f16, volatile>(%4)));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
