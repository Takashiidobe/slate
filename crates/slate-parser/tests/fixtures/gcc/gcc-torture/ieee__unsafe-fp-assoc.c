/* { dg-do run } */
#include <float.h>

extern void abort(void);

static const double C = DBL_MAX;

double foo(double x) { return (((x * C) * C) * C); }

int main() {
  double d = foo(0.0);
  if (d != 0.0)
    abort();

  return 0;
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
// DEFAULT-NEXT:     global %1 C: f64 [storage=static] [const] = const<f64>(1.7976931348623157e308) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%3), read<f64>(%1)), read<f64>(%1)), read<f64>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 d: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%2, const<f64>(0.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%5), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
