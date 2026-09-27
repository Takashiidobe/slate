/* PR target/79487 */
/* { dg-options "-O2" } */

int
main ()
{
  _Decimal32 a = (-9223372036854775807LL - 1LL); 
  _Decimal32 b = -9.223372E+18DF;
  if (b - a != 0.0DF)
    __builtin_abort ();
  _Decimal64 c = (-9223372036854775807LL - 1LL); 
  _Decimal64 d = -9.223372036854776E+18DD;
  if (d - c != 0.0DD)
    __builtin_abort ();
  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %5 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 a: d32 [storage=automatic] = int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1)));
// DEFAULT-NEXT:         let %2 b: d32 [storage=automatic] = neg<d32>(const<d32>(9.223372e+18));
// DEFAULT-NEXT:         if ne<d32, exceptions=observable>(sub<d32, rounding=nearest_even, exceptions=observable, contract=fast>(read<d32>(%2), read<d32>(%1)), const<d32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         let %3 c: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1)));
// DEFAULT-NEXT:         let %4 d: d64 [storage=automatic] = neg<d64>(const<d64>(9.223372036854776e+18));
// DEFAULT-NEXT:         if ne<d64, exceptions=observable>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%4), read<d64>(%3)), const<d64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
