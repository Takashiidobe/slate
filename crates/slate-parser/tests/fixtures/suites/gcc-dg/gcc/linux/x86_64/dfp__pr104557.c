/* PR debug/104557 */
/* { dg-do compile } */
/* { dg-options "-O -g -Wno-psabi" } */

typedef int __attribute__((__vector_size__ (32))) U;
typedef double __attribute__((__vector_size__ (32))) F;
typedef _Decimal64 __attribute__((__vector_size__ (32))) D;

F
bar (void)
{
  F f = __builtin_convertvector ((D) (-10.d < (D) ((D) (U) { 0, 0, 0, 0, 0, 0, 0, -0xe0 }
						   >= (D) { 80000000 })), F);
  return f;
}

F
foo ()
{
  F x = bar ();
  return x;
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
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = vector<i32, 8>;
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = vector<f64, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = vector<d64, 4>;
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> vector<f64, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: vector<f64, 4> [storage=automatic] = float_convert<vector<f64, 4>, reason=explicit, rounding=nearest_even, exceptions=observable>(vector_bit_cast<vector<d64, 4>, reason=explicit>(vector_bit_cast<vector<d64, 4>, reason=explicit>(lt<vector<d64, 4>, result=vector<i64, 4>, exceptions=observable>(vector_splat<vector<d64, 4>, reason=usual_arith>(float_convert<d64, reason=usual_arith, rounding=nearest_even, exceptions=observable>(neg<f64>(const<f64>(10.0)))), vector_bit_cast<vector<d64, 4>, reason=explicit>(ge<vector<d64, 4>, result=vector<i64, 4>, exceptions=observable>(vector_bit_cast<vector<d64, 4>, reason=explicit>(read<vector<i32, 8>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<i32, 8>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0), index2 = const<i32>(0), index3 = const<i32>(0), index4 = const<i32>(0), index5 = const<i32>(0), index6 = const<i32>(0), index7 = neg<i32, overflow=ub>(const<i32>(224))))), read<vector<d64, 4>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<vector<d64, 4>, zero_fill=true>(index0 = int_to_float<d64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(80000000))))))))));
// DEFAULT-NEXT:         return read<vector<f64, 4>>(%[[VALUE_f]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> vector<f64, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: vector<f64, 4> [storage=automatic] = call<vector<f64, 4>, signature=fn() -> vector<f64, 4>, abi=sysv64() -> direct>(%[[VALUE_bar]]);
// DEFAULT-NEXT:         return read<vector<f64, 4>>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
