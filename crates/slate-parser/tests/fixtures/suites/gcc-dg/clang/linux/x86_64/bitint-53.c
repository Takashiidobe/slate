/* PR tree-optimization/112940 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

#if __BITINT_MAXWIDTH__ >= 1025
_BitInt (1025) b;
#endif

void
foo (long x)
{
#if __BITINT_MAXWIDTH__ >= 1025
  b += (unsigned _BitInt (255)) x;
#else
  (void) x;
#endif
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
// DEFAULT-NEXT:     global %0 b: i1025b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3: i1025b [synthetic] = read<i1025b>(%0);
// DEFAULT-NEXT:         let %4: i1025b [synthetic] = add<i1025b, overflow=ub>(read<i1025b>(%3), reinterpret<i1025b, reason=usual_arith, fits=unknown>(widen<u1025b, reason=usual_arith>(reinterpret<u255b, reason=explicit, fits=unknown>(widen<i255b, reason=explicit>(read<i64>(%2))))));
// DEFAULT-NEXT:         write<i1025b>(%0, read<i1025b>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
