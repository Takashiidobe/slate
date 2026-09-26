/* { dg-do run }
   { dg-xfail-if "" { *-*-vxworks* } }
   { dg-xfail-if "" { tic6x-*-* && ti_c67x } } */
#include <math.h>

void abort(void);
void exit(int);

static const double zero = 0.0;
static const double pone = 1.0;
static const double none = -1.0;
static const double pinf = 1.0 / 0.0;
static const double ninf = -1.0 / 0.0;

int main() {
  if (pinf != pone / zero)
    abort();

  if (ninf != none / zero)
    abort();

#ifdef HUGE_VAL
  if (HUGE_VAL != pinf)
    abort();

  if (-HUGE_VAL != ninf)
    abort();
#endif

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
// DEFAULT-NEXT:     global %2 zero: f64 [storage=static] [const] = const<f64>(0.0) [linkage=internal];
// DEFAULT-NEXT:     global %3 pone: f64 [storage=static] [const] = const<f64>(1.0) [linkage=internal];
// DEFAULT-NEXT:     global %4 none: f64 [storage=static] [const] = neg<f64>(const<f64>(1.0)) [linkage=internal];
// DEFAULT-NEXT:     global %5 pinf: f64 [storage=static] [const] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %6 ninf: f64 [storage=static] [const] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(neg<f64>(const<f64>(1.0)), const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%5), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%3), read<f64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%6), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%4), read<f64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn() -> f64>(__builtin_huge_val), read<f64>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(neg<f64>(call<f64, signature=fn() -> f64>(__builtin_huge_val)), read<f64>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
