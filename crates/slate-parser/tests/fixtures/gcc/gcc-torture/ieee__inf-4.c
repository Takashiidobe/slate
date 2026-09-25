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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: f64, %2 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 c: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%1), read<f64>(%2));
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=finite>(read<f64>(%3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if float_class<bool, test=nan>(read<f64>(%3))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if logical_and<bool>(not<bool>(float_class<bool, test=nan>(read<f64>(%1))), not<bool>(float_class<bool, test=nan>(read<f64>(%2))))
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_and<bool>(float_class<bool, test=finite>(read<f64>(%1)), float_class<bool, test=finite>(read<f64>(%2)))
// DEFAULT-NEXT:                         return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<f64, exceptions=ignore>(read<f64>(%3), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<f64, exceptions=ignore>(read<f64>(%1), read<f64>(%2)))
// DEFAULT-NEXT:                 return const<i32>(3);
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 a: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(__builtin_inf);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64, f64) -> i32>(%0, read<f64>(%5), read<f64>(%5)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
