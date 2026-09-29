/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_BitInt(2)
foo (_BitInt(2) x, _BitInt(15) y)
{
  return x + y;
}

_BitInt(64)
bar (_BitInt(64) x, _BitInt(64) y)
{
  return x + y;
}

#if __BITINT_MAXWIDTH__ >= 128
_BitInt(128) a, b, c;

_BitInt(128)
baz (_BitInt(128) x, _BitInt(128) y)
{
  a = x;
  b = y;
  return c;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
_BitInt(575) d, e, f;

_BitInt(575)
qux (_BitInt(575) x, _BitInt(575) y)
{
  d = x;
  e = y;
  return f;
}
#endif

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i2b, %[[VALUE_y:[0-9]+]] y: i15b) -> i2b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i2b, reason=return, fits=unknown>(add<i15b, overflow=ub>(widen<i15b, reason=usual_arith>(read<i2b>(%[[VALUE_x]])), read<i15b>(%[[VALUE_y]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i64b, %[[VALUE_y_2:[0-9]+]] y: i64b) -> i64b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i64b, overflow=ub>(read<i64b>(%[[VALUE_x_2]]), read<i64b>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: i128b, %[[VALUE_y_3:[0-9]+]] y: i128b) -> i128b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i128b>(%[[VALUE_a]], read<i128b>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         write<i128b>(%[[VALUE_b]], read<i128b>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:         return read<i128b>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_x_4:[0-9]+]] x: i575b, %[[VALUE_y_4:[0-9]+]] y: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_d]], read<i575b>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         write<i575b>(%[[VALUE_e]], read<i575b>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:         return read<i575b>(%[[VALUE_f]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
