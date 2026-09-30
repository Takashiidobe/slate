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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl:[0-9]+]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_valf:[0-9]+]] @__builtin_huge_valf() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_val:[0-9]+]] @__builtin_huge_val() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_vall:[0-9]+]] @__builtin_huge_vall() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_fi:[0-9]+]] fi: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]);
// DEFAULT-NEXT:         let %[[VALUE_di:[0-9]+]] di: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]);
// DEFAULT-NEXT:         let %[[VALUE_li:[0-9]+]] li: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]);
// DEFAULT-NEXT:         let %[[VALUE_fh:[0-9]+]] fh: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%[[VALUE___builtin_huge_valf]]);
// DEFAULT-NEXT:         let %[[VALUE_dh:[0-9]+]] dh: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_huge_val]]);
// DEFAULT-NEXT:         let %[[VALUE_lh:[0-9]+]] lh: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_huge_vall]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_fi]]), read<f32>(%[[VALUE_fi]])), read<f32>(%[[VALUE_fi]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_di]]), read<f64>(%[[VALUE_di]])), read<f64>(%[[VALUE_di]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_li]]), read<f80>(%[[VALUE_li]])), read<f80>(%[[VALUE_li]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_fi]]), read<f32>(%[[VALUE_fh]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_di]]), read<f64>(%[[VALUE_dh]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_li]]), read<f80>(%[[VALUE_lh]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<f32, exceptions=ignore>(read<f32>(%[[VALUE_fi]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<f64, exceptions=ignore>(read<f64>(%[[VALUE_di]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<f80, exceptions=ignore>(read<f80>(%[[VALUE_li]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
