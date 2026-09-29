/* PR middle-end/116899 */
/* { dg-do compile { target bitint575 } } */
/* { dg-options "-O2" } */

float f;
_BitInt(255) b;

void
foo (signed char c)
{
  for (;;)
    {
      c %= (unsigned _BitInt(512)) 0;	/* { dg-warning "division by zero" } */
      f /= b >= c;
    }
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
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i255b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_c]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i8 [synthetic] = reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(rem<u512b, by_zero=ub>(reinterpret<u512b, reason=usual_arith, fits=unknown>(widen<i512b, reason=usual_arith>(widen<i32, reason=promotion>(read<i8>(%[[VALUE1]])))), reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                     write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE2]]));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_f]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: f32 [synthetic] = div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE3]]), int_to_float<f32, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=promotion>(ge<i255b>(read<i255b>(%[[VALUE_b]]), widen<i255b, reason=usual_arith>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])))))));
// DEFAULT-NEXT:                     write<f32>(%[[VALUE_f]], read<f32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
