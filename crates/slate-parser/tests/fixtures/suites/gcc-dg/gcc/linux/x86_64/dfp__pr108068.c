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
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 x: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         while %3 ne<d64, exceptions=observable>(read<d64>(%1), int_to_float<d64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:             let %5: d64 [synthetic] = read<d64>(%1);
// DEFAULT-NEXT:             let %6: d64 [synthetic] = div<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%5), int_to_float<d64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(10)));
// DEFAULT-NEXT:             write<d64>(%1, read<d64>(%6));
// DEFAULT-NEXT:         let %2 d: f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(read<d64>(%1));
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=sign_bit>(read<f64>(%2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
