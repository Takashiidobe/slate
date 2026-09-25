/* { dg-do run } */
extern void abort(void);

int main() {
  float       fi = __builtin_inff();
  double      di = __builtin_inf();
  long double li = __builtin_infl();

  float       fh = __builtin_huge_valf();
  double      dh = __builtin_huge_val();
  long double lh = __builtin_huge_vall();

  if (fi + fi != fi)
    abort();
  if (di + di != di)
    abort();
  if (li + li != li)
    abort();

  if (fi != fh)
    abort();
  if (di != dh)
    abort();
  if (li != lh)
    abort();

  if (fi <= 0)
    abort();
  if (di <= 0)
    abort();
  if (li <= 0)
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
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 fi: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(__builtin_inff);
// DEFAULT-NEXT:         let %3 di: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(__builtin_inf);
// DEFAULT-NEXT:         let %4 li: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(__builtin_infl);
// DEFAULT-NEXT:         let %5 fh: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(__builtin_huge_valf);
// DEFAULT-NEXT:         let %6 dh: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(__builtin_huge_val);
// DEFAULT-NEXT:         let %7 lh: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(__builtin_huge_vall);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%2), read<f32>(%2)), read<f32>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%3), read<f64>(%3)), read<f64>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(add<f80, rounding=nearest_even, exceptions=ignore>(read<f80>(%4), read<f80>(%4)), read<f80>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(read<f32>(%2), read<f32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%3), read<f64>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%4), read<f80>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if le<f32, exceptions=ignore>(read<f32>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if le<f64, exceptions=ignore>(read<f64>(%3), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if le<f80, exceptions=ignore>(read<f80>(%4), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
