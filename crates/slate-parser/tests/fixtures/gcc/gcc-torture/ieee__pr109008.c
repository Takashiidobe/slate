/* { dg-do run } */
/* PR tree-optimization/109008 */

__attribute__((noipa)) double foo(double eps) {
  double d = 1. + eps;
  if (d == 1.)
    return eps;
  return 0.0;
}

int main() {
  if (foo(__DBL_EPSILON__ / 8.0) == 0.0)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%1 eps: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 d: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%1));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%2), const<f64>(1.0))
// DEFAULT-NEXT:             return read<f64>(%1);
// DEFAULT-NEXT:         return const<f64>(0.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%0, div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.220446049250313e-16), const<f64>(8.0))), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
