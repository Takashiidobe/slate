/* PR middle-end/120631 */
/* { dg-options "-O2" } */

_Decimal64 a = 1234567891357900000.0dd;
long long b = 1234567891357900000LL;
_Decimal32 c = 1234567000000000000.0df;
long long d = 1234567000000000000LL;

int
main ()
{
  if (a != b || (long long) a != b || c != d || (long long) c != d)
    __builtin_abort ();
  _Decimal64 e = 1234567891357900000.0dd;
  long long f = 1234567891357900000LL;
  _Decimal64 g = 1234567891357900000LL;
  long long h = 1234567891357900000.0dd;
  _Decimal32 i = 1234567000000000000.0df;
  long long j = 1234567000000000000LL;
  _Decimal32 k = 1234567000000000000LL;
  long long l = 1234567000000000000.0df;
  if (e != g || f != h || i != k || j != l)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: d64 [storage=static] = const<d64>(1234567891357900000.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i64 [storage=static] = const<i64>(1234567891357900000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: d32 [storage=static] = const<d32>(1234567000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i64 [storage=static] = const<i64>(1234567000000000000) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), int_to_float<d64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE_b]]))), ne<i64>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_a]])), read<i64>(%[[VALUE_b]]))), ne<d32, exceptions=observable>(read<d32>(%[[VALUE_c]]), int_to_float<d32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE_d]])))), ne<i64>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_c]])), read<i64>(%[[VALUE_d]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: d64 [storage=automatic] = const<d64>(1234567891357900000.0);
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i64 [storage=automatic] = const<i64>(1234567891357900000);
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i64>(1234567891357900000));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i64 [storage=automatic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(const<d64>(1234567891357900000.0));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: d32 [storage=automatic] = const<d32>(1234567000000000000.0);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i64 [storage=automatic] = const<i64>(1234567000000000000);
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: d32 [storage=automatic] = int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i64>(1234567000000000000));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i64 [storage=automatic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(const<d32>(1234567000000000000.0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d64, exceptions=observable>(read<d64>(%[[VALUE_e]]), read<d64>(%[[VALUE_g]])), ne<i64>(read<i64>(%[[VALUE_f]]), read<i64>(%[[VALUE_h]]))), ne<d32, exceptions=observable>(read<d32>(%[[VALUE_i]]), read<d32>(%[[VALUE_k]]))), ne<i64>(read<i64>(%[[VALUE_j]]), read<i64>(%[[VALUE_l]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
