/* Test _Float16 type-generic built-in functions: __builtin_isinf_sign.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float16 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float16_runtime } */

#define WIDTH 16
#define EXT   0
/* Tests for _FloatN / _FloatNx types: compile and execution tests for
   type-generic built-in functions: __builtin_isinf_sign.  Before
   including this file, define WIDTH as the value N; define EXT to 1
   for _FloatNx and 0 for _FloatN.  */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

#define CONCATX(X, Y)       X##Y
#define CONCAT(X, Y)        CONCATX(X, Y)
#define CONCAT3(X, Y, Z)    CONCAT(CONCAT(X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT(CONCAT(CONCAT(W, X), Y), Z)

#if EXT
#define TYPE   CONCAT3(_Float, WIDTH, x)
#define CST(C) CONCAT4(C, f, WIDTH, x)
#define MAX    CONCAT3(FLT, WIDTH, X_MAX)
#else
#define TYPE   CONCAT(_Float, WIDTH)
#define CST(C) CONCAT3(C, f, WIDTH)
#define MAX    CONCAT3(FLT, WIDTH, _MAX)
#endif

extern void exit(int);
extern void abort(void);

volatile TYPE inf = __builtin_inf(), nanval = __builtin_nan("");
volatile TYPE neginf = -__builtin_inf(), negnanval = -__builtin_nan("");
volatile TYPE zero = CST(0.0), negzero = -CST(0.0), one = CST(1.0);
volatile TYPE max = MAX, negmax = -MAX;

int
main(void) {
  if (__builtin_isinf_sign(inf) != 1)
    abort();
  if (__builtin_isinf_sign(neginf) != -1)
    abort();
  if (__builtin_isinf_sign(nanval) != 0)
    abort();
  if (__builtin_isinf_sign(negnanval) != 0)
    abort();
  if (__builtin_isinf_sign(zero) != 0)
    abort();
  if (__builtin_isinf_sign(negzero) != 0)
    abort();
  if (__builtin_isinf_sign(one) != 0)
    abort();
  if (__builtin_isinf_sign(max) != 0)
    abort();
  if (__builtin_isinf_sign(negmax) != 0)
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
// DEFAULT-NEXT:     global %2 inf: volatile f16 [storage=static] = float_narrow<f16, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn() -> f64>(%13)) [linkage=external];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 nanval: volatile f16 [storage=static] = float_narrow<f16, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>) -> f64>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%16)))) [linkage=external];
// DEFAULT-NEXT:     global %4 neginf: volatile f16 [storage=static] = float_narrow<f16, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(call<f64, signature=fn() -> f64>(%13))) [linkage=external];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 negnanval: volatile f16 [storage=static] = float_narrow<f16, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(call<f64, signature=fn(ptr<const i8>) -> f64>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%17))))) [linkage=external];
// DEFAULT-NEXT:     global %6 zero: volatile f16 [storage=static] = const<f16>(0) [linkage=external];
// DEFAULT-NEXT:     global %7 negzero: volatile f16 [storage=static] = neg<f16>(const<f16>(0)) [linkage=external];
// DEFAULT-NEXT:     global %8 one: volatile f16 [storage=static] = const<f16>(1) [linkage=external];
// DEFAULT-NEXT:     global %9 max: volatile f16 [storage=static] = const<f16>(65504) [linkage=external];
// DEFAULT-NEXT:     global %10 negmax: volatile f16 [storage=static] = neg<f16>(const<f16>(65504)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%12 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @__builtin_inf() -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_nan(%14 <unnamed>: ptr<const i8>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%2)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%2)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%4)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%4)), const<i32>(-1), const<i32>(1)), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%3)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%3)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%5)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%5)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%6)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%6)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%7)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%7)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%8)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%8)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%9)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%9)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(conditional<i32>(float_class<bool, test=infinite>(read<f16, volatile>(%10)), conditional<i32>(float_class<bool, test=sign_bit>(read<f16, volatile>(%10)), const<i32>(-1), const<i32>(1)), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
