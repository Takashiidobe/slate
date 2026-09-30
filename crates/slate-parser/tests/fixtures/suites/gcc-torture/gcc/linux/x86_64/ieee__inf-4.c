/* { dg-do run } */
__attribute__((noipa)) int foo(double a, double b) {
  double c = a - b;
  if (!__builtin_isfinite(c)) {
    if (__builtin_isnan(c)) {
      if (!__builtin_isnan(a) && !__builtin_isnan(b))
        return 1;
    } else if (__builtin_isfinite(a) && __builtin_isfinite(b))
      return 2;
  } else if (c == 0 && a != b)
    return 3;
  return 4;
}

int main() {
  double a = __builtin_inf();
  if (foo(a, a) != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=finite>(read<f64>(%[[VALUE_c]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if float_class<bool, test=nan>(read<f64>(%[[VALUE_c]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if logical_and<bool>(not<bool>(float_class<bool, test=nan>(read<f64>(%[[VALUE_a]]))), not<bool>(float_class<bool, test=nan>(read<f64>(%[[VALUE_b]]))))
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_and<bool>(float_class<bool, test=finite>(read<f64>(%[[VALUE_a]])), float_class<bool, test=finite>(read<f64>(%[[VALUE_b]])))
// DEFAULT-NEXT:                         return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<f64, exceptions=observable>(read<f64>(%[[VALUE_c]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), ne<f64, exceptions=observable>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])))
// DEFAULT-NEXT:                 return const<i32>(3);
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64, f64) -> i32>(%[[VALUE_foo]], read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_a_2]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
