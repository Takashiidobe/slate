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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %6 a: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 b: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 c: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 d: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 e: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 f: i575b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i2b, %2 y: i15b) -> i2b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i2b, reason=return, fits=unknown>(add<i15b, overflow=ub>(widen<i15b, reason=usual_arith>(read<i2b>(%1)), read<i15b>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: i64b, %5 y: i64b) -> i64b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i64b, overflow=ub>(read<i64b>(%4), read<i64b>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @baz(%10 x: i128b, %11 y: i128b) -> i128b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i128b>(%6, read<i128b>(%10));
// DEFAULT-NEXT:         write<i128b>(%7, read<i128b>(%11));
// DEFAULT-NEXT:         return read<i128b>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @qux(%16 x: i575b, %17 y: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i575b>(%12, read<i575b>(%16));
// DEFAULT-NEXT:         write<i575b>(%13, read<i575b>(%17));
// DEFAULT-NEXT:         return read<i575b>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
