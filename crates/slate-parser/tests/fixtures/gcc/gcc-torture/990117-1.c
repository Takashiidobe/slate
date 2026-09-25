void abort(void);

int foo(int x, int y, int i, int j) {
  double tmp1 = ((double)x / y);
  double tmp2 = ((double)i / j);

  return tmp1 < tmp2;
}

int main(void) {
  if (foo(2, 24, 3, 4) == 0)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i32, %3 y: i32, %4 i: i32, %5 j: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 tmp1: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%2)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%3)));
// DEFAULT-NEXT:         let %7 tmp2: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%4)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%5)));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=ignore>(read<f64>(%6), read<f64>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(i32, i32, i32, i32) -> i32>(%1, const<i32>(2), const<i32>(24), const<i32>(3), const<i32>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
