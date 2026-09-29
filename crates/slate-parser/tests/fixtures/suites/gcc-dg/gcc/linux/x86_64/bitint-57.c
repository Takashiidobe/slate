/* PR tree-optimization/112941 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O1 -fno-tree-forwprop" } */

#if __BITINT_MAXWIDTH__ >= 6384
unsigned _BitInt(2049)
foo (unsigned _BitInt(6384) x, _BitInt(8) y)
{
  unsigned _BitInt(6384) z = y;
  return x * z;
}

_BitInt(2049)
bar (unsigned _BitInt(6384) x, _BitInt(1023) y)
{
  unsigned _BitInt(6384) z = y;
  return x * z;
}
#else
int i;
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u6384b, %[[VALUE_y:[0-9]+]] y: i8b) -> u2049b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: u6384b [storage=automatic] = reinterpret<u6384b, reason=assign, fits=unknown>(widen<i6384b, reason=assign>(read<i8b>(%[[VALUE_y]])));
// DEFAULT-NEXT:         return truncate<u2049b, reason=return, fits=unknown>(mul<u6384b, overflow=wrap>(read<u6384b>(%[[VALUE_x]]), read<u6384b>(%[[VALUE_z]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: u6384b, %[[VALUE_y_2:[0-9]+]] y: i1023b) -> i2049b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: u6384b [storage=automatic] = reinterpret<u6384b, reason=assign, fits=unknown>(widen<i6384b, reason=assign>(read<i1023b>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:         return reinterpret<i2049b, reason=return, fits=unknown>(truncate<u2049b, reason=return, fits=unknown>(mul<u6384b, overflow=wrap>(read<u6384b>(%[[VALUE_x_2]]), read<u6384b>(%[[VALUE_z_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
