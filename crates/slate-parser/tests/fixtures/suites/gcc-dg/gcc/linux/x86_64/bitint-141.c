/* PR middle-end/126497 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23" } */

typedef unsigned _BitInt (1) U;

U
foo (U a)
{
  U t = a >= 1uwb;
  return t;
}

U
bar (U a)
{
  U t = a == 1uwb;
  return t;
}

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
// DEFAULT-NEXT:     type @type0 U = u1b;
// DEFAULT-NEXT:     fn %1 @foo(%2 a: u1b) -> u1b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 t: u1b [storage=automatic] = from_bool<u1b, reason=assign>(ge<u1b>(read<u1b>(%2), const<u1b>(1)));
// DEFAULT-NEXT:         return read<u1b>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 a: u1b) -> u1b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 t: u1b [storage=automatic] = from_bool<u1b, reason=assign>(eq<u1b>(read<u1b>(%5), const<u1b>(1)));
// DEFAULT-NEXT:         return read<u1b>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
