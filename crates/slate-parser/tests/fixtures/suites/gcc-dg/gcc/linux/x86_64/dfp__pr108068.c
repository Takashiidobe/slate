/* PR tree-optimization/108068 */
/* { dg-options "-O2" } */

int
main ()
{
  _Decimal64 x = -1;
  while (x != 0)
    x /= 10;
  double d = x;
  if (!__builtin_signbit (d))
    __builtin_abort ();
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<d64, exceptions=observable>(read<d64>(%[[VALUE_x]]), int_to_float<d64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: d64 [synthetic] = read<d64>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: d64 [synthetic] = div<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE1]]), int_to_float<d64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(10)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_x]], read<d64>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(read<d64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=sign_bit>(read<f64>(%[[VALUE_d]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
