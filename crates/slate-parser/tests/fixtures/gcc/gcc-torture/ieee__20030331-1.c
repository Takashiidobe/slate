/* { dg-do run } */
extern void exit(int);
extern void abort(void);
float       x = -1.5f;

float rintf() {
  static const float TWO23 = 8388608.0;

  if (__builtin_fabs(x) < TWO23) {
    if (x > 0.0) {
      x += TWO23;
      x -= TWO23;
    } else if (x < 0.0) {
      x = TWO23 - x;
      x = -(x - TWO23);
    }
  }

  return x;
}

int main(void) {
  if (rintf() != -2.0)
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
// DEFAULT-NEXT:     global %2 x: f32 [storage=static] = neg<f32>(const<f32>(1.5)) [linkage=external];
// DEFAULT-NEXT:     global %4 TWO23: f32 [storage=static] [const] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(8388608.0)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @rintf() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(__builtin_fabs, float_widen<f64, reason=arg>(read<f32>(%2))), float_widen<f64, reason=usual_arith>(read<f32>(%4)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if gt<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%2)), const<f64>(0.0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %7: f32 [synthetic] = read<f32>(%2);
// DEFAULT-NEXT:                         let %8: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%7), read<f32>(%4));
// DEFAULT-NEXT:                         write<f32>(%2, read<f32>(%8));
// DEFAULT-NEXT:                         let %9: f32 [synthetic] = read<f32>(%2);
// DEFAULT-NEXT:                         let %10: f32 [synthetic] = sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%9), read<f32>(%4));
// DEFAULT-NEXT:                         write<f32>(%2, read<f32>(%10));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%2)), const<f64>(0.0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<f32>(%2, sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%4), read<f32>(%2)));
// DEFAULT-NEXT:                             write<f32>(%2, neg<f32>(sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%2), read<f32>(%4))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<f32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn() -> f32>(%3)), neg<f64>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
