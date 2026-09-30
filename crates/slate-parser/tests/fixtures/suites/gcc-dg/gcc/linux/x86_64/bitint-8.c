/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23 -Wno-uninitialized" } */

#if __BITINT_MAXWIDTH__ >= 135
_BitInt(135)
foo (void)
{
  _BitInt(135) d;
  _BitInt(135) e = d + 2wb;
  return e;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
_BitInt(575)
bar (void)
{
  _BitInt(575) d;
  _BitInt(575) e = d * 42wb;
  return e;
}

_BitInt(575)
baz (int x)
{
  _BitInt(575) d;
  if (x)
    return 59843758943759843574wb;
  return d;
}
#endif

int x;

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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i135b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i135b [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i135b [storage=automatic] = add<i135b, overflow=ub>(read<i135b>(%[[VALUE_d]]), widen<i135b, reason=usual_arith>(const<i3b>(2)));
// DEFAULT-NEXT:         return read<i135b>(%[[VALUE_e]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: i575b [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: i575b [storage=automatic] = mul<i575b, overflow=ub>(read<i575b>(%[[VALUE_d_2]]), widen<i575b, reason=usual_arith>(const<i7b>(42)));
// DEFAULT-NEXT:         return read<i575b>(%[[VALUE_e_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_2:[0-9]+]] x: i32) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d_3:[0-9]+]] d: i575b [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// DEFAULT-NEXT:             return widen<i575b, reason=return>(const<i67b>(59843758943759843574));
// DEFAULT-NEXT:         return read<i575b>(%[[VALUE_d_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
