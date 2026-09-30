int knr(a, b, c, d)
char a;
short b;
float c;
{
  return a + b + c + d;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

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
// DEFAULT-NEXT:     fn %[[VALUE_knr:[0-9]+]] @knr(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: f64, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i8 [storage=automatic] = truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: i16 [storage=automatic] = truncate<i16, reason=arg, fits=unknown>(read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: f32 [storage=automatic] = float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f64>(%[[VALUE_c]]));
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_a_2]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_b_2]])))), read<f32>(%[[VALUE_c_2]])), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_d]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
