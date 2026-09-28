/* Test _Float16.  */
/* { dg-do run } */
/* { dg-options "-Wno-old-style-definition" } */
/* { dg-add-options float16 } */
/* { dg-require-effective-target float16_runtime } */

#define WIDTH 16
#define EXT   0
/* Basic tests for _FloatN / _FloatNx types: compile and execution
   tests for valid code.  Before including this file, define WIDTH as
   the value N; define EXT to 1 for _FloatNx and 0 for _FloatN.  */

#include <stdarg.h>

#define CONCATX(X, Y)       X##Y
#define CONCAT(X, Y)        CONCATX(X, Y)
#define CONCAT3(X, Y, Z)    CONCAT(CONCAT(X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT(CONCAT(CONCAT(W, X), Y), Z)

#ifndef TYPE
#if EXT
#define TYPE    CONCAT3(_Float, WIDTH, x)
#define CST(C)  CONCAT4(C, f, WIDTH, x)
#define CSTU(C) CONCAT4(C, F, WIDTH, x)
#else
#define TYPE    CONCAT(_Float, WIDTH)
#define CST(C)  CONCAT3(C, f, WIDTH)
#define CSTU(C) CONCAT3(C, F, WIDTH)
#endif
#endif

extern void exit(int);
extern void abort(void);

volatile TYPE a = CST(1.0), b = CSTU(2.5), c = -CST(2.5);
volatile TYPE a2 = CST(1.0), z = CST(0.0), nz = -CST(0.0);

/* These types are not subject to default argument promotions.  */

TYPE vafn(TYPE arg1, ...) {
  va_list ap;
  TYPE    ret;
  va_start(ap, arg1);
  ret = arg1 + va_arg(ap, TYPE);
  va_end(ap);
  return ret;
}

TYPE krfn(TYPE arg) {
  return arg + 1;
}

TYPE krprofn(TYPE);
TYPE krprofn(TYPE arg) {
  return arg * 3;
}

TYPE profn(TYPE arg) { return arg / 4; }

int main(void) {
  volatile TYPE r;
  r = -b;
  if (r != c)
    abort();
  r = a + b;
  if (r != CST(3.5))
    abort();
  r = a - b;
  if (r != -CST(1.5))
    abort();
  r = 2 * c;
  if (r != -5)
    abort();
  r = b * c;
  if (r != -CST(6.25))
    abort();
  r = b / (a + a);
  if (r != CST(1.25))
    abort();
  r = c * 3;
  if (r != -CST(7.5))
    abort();
  volatile int i = r;
  if (i != -7)
    abort();
  r = vafn(a, c);
  if (r != -CST(1.5))
    abort();
  r = krfn(b);
  if (r != CST(3.5))
    abort();
  r = krprofn(a);
  if (r != CST(3.0))
    abort();
  r = profn(a);
  if (r != CST(0.25))
    abort();
  if ((a < b) != 1)
    abort();
  if ((b < a) != 0)
    abort();
  if ((a < a2) != 0)
    abort();
  if ((nz < z) != 0)
    abort();
  if ((a <= b) != 1)
    abort();
  if ((b <= a) != 0)
    abort();
  if ((a <= a2) != 1)
    abort();
  if ((nz <= z) != 1)
    abort();
  if ((a > b) != 0)
    abort();
  if ((b > a) != 1)
    abort();
  if ((a > a2) != 0)
    abort();
  if ((nz > z) != 0)
    abort();
  if ((a >= b) != 0)
    abort();
  if ((b >= a) != 1)
    abort();
  if ((a >= a2) != 1)
    abort();
  if ((nz >= z) != 1)
    abort();
  i = (nz == z);
  if (i != 1)
    abort();
  i = (a == b);
  if (i != 0)
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     global %3 a: volatile f16 [storage=static] = const<f16>(1) [linkage=external];
// DEFAULT-NEXT:     global %4 b: volatile f16 [storage=static] = const<f16>(2.5) [linkage=external];
// DEFAULT-NEXT:     global %5 c: volatile f16 [storage=static] = neg<f16>(const<f16>(2.5)) [linkage=external];
// DEFAULT-NEXT:     global %6 a2: volatile f16 [storage=static] = const<f16>(1) [linkage=external];
// DEFAULT-NEXT:     global %7 z: volatile f16 [storage=static] = const<f16>(0) [linkage=external];
// DEFAULT-NEXT:     global %8 nz: volatile f16 [storage=static] = neg<f16>(const<f16>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%22 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @vafn(%10 arg1: f16, ...) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %12 ret: f16 [storage=automatic];
// DEFAULT-NEXT:         va_start(%11);
// DEFAULT-NEXT:         write<f16>(%12, add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%10), va_arg<f16>(%11)));
// DEFAULT-NEXT:         add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%10), va_arg<f16>(%11));
// DEFAULT-NEXT:         va_end(%11);
// DEFAULT-NEXT:         return read<f16>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @krfn(%14 arg: f16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%14), int_to_float<f16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @krprofn(%16 arg: f16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%16), int_to_float<f16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @profn(%18 arg: f16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16>(%18), int_to_float<f16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %20 r: volatile f16 [storage=automatic];
// DEFAULT-NEXT:         write<f16, volatile>(%20, neg<f16>(read<f16, volatile>(%4)));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), read<f16, volatile>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16, volatile>(%3), read<f16, volatile>(%4)));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), const<f16>(3.5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, sub<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16, volatile>(%3), read<f16, volatile>(%4)));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), neg<f16>(const<f16>(1.5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, mul<f16, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), read<f16, volatile>(%5)));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), int_to_float<f16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, mul<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16, volatile>(%4), read<f16, volatile>(%5)));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), neg<f16>(const<f16>(6.25)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, div<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16, volatile>(%4), add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16, volatile>(%3), read<f16, volatile>(%3))));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), const<f16>(1.25))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, mul<f16, rounding=nearest_even, exceptions=ignore, contract=on>(read<f16, volatile>(%5), int_to_float<f16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3))));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), neg<f16>(const<f16>(7.5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         let %21 i: volatile i32 [storage=automatic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f16, volatile>(%20));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%21), neg<i32, overflow=ub>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, call<f16, signature=fn(f16, ...) -> f16>(%9, read<f16, volatile>(%3), read<f16, volatile>(%5)));
// DEFAULT-NEXT:         call<f16, signature=fn(f16, ...) -> f16>(%9, read<f16, volatile>(%3), read<f16, volatile>(%5));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), neg<f16>(const<f16>(1.5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, call<f16, signature=fn(f16) -> f16>(%13, read<f16, volatile>(%4)));
// DEFAULT-NEXT:         call<f16, signature=fn(f16) -> f16>(%13, read<f16, volatile>(%4));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), const<f16>(3.5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, call<f16, signature=fn(f16) -> f16>(%15, read<f16, volatile>(%3)));
// DEFAULT-NEXT:         call<f16, signature=fn(f16) -> f16>(%15, read<f16, volatile>(%3));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), const<f16>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<f16, volatile>(%20, call<f16, signature=fn(f16) -> f16>(%17, read<f16, volatile>(%3)));
// DEFAULT-NEXT:         call<f16, signature=fn(f16) -> f16>(%17, read<f16, volatile>(%3));
// DEFAULT-NEXT:         if ne<f16, exceptions=ignore>(read<f16, volatile>(%20), const<f16>(0.25))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(lt<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%4))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(lt<f16, exceptions=ignore>(read<f16, volatile>(%4), read<f16, volatile>(%3))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(lt<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%6))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(lt<f16, exceptions=ignore>(read<f16, volatile>(%8), read<f16, volatile>(%7))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(le<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%4))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(le<f16, exceptions=ignore>(read<f16, volatile>(%4), read<f16, volatile>(%3))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(le<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%6))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(le<f16, exceptions=ignore>(read<f16, volatile>(%8), read<f16, volatile>(%7))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(gt<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%4))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(gt<f16, exceptions=ignore>(read<f16, volatile>(%4), read<f16, volatile>(%3))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(gt<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%6))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(gt<f16, exceptions=ignore>(read<f16, volatile>(%8), read<f16, volatile>(%7))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(ge<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%4))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(ge<f16, exceptions=ignore>(read<f16, volatile>(%4), read<f16, volatile>(%3))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(ge<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%6))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(ge<f16, exceptions=ignore>(read<f16, volatile>(%8), read<f16, volatile>(%7))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<i32, volatile>(%21, from_bool<i32, reason=assign>(eq<f16, exceptions=ignore>(read<f16, volatile>(%8), read<f16, volatile>(%7))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%21), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<i32, volatile>(%21, from_bool<i32, reason=assign>(eq<f16, exceptions=ignore>(read<f16, volatile>(%3), read<f16, volatile>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%21), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
