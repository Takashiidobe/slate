/* Test C23 definition of LDBL_EPSILON.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <float.h>

extern void abort(void);
extern void exit(int);

int main(void) {
  volatile long double x = 1.0L;
  for (int i = 0; i < LDBL_MANT_DIG - 1; i++)
    x /= 2;
  if (x != LDBL_EPSILON)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 x: volatile f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), sub<i32, overflow=ub>(const<i32>(64), const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %9: f80 [synthetic] = read<f80, volatile>(%3);
// DEFAULT-NEXT:                 let %10: f80 [synthetic] = div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%9), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)));
// DEFAULT-NEXT:                 write<f80, volatile>(%3, read<f80>(%10));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80, volatile>(%3), const<f80>(1.08420217248550443401E-19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
