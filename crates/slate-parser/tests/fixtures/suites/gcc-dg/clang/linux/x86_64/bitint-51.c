/* PR tree-optimization/112901 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */

float f;
#if __BITINT_MAXWIDTH__ >= 256
_BitInt(256) i;

void
foo (void)
{
  f *= 4 * i;
}
#endif

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
// DEFAULT-NEXT:     global %0 f: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 i: i256b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3: f32 [synthetic] = read<f32>(%0);
// DEFAULT-NEXT:         let %4: f32 [synthetic] = mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(mul<i256b, overflow=ub>(widen<i256b, reason=usual_arith>(const<i32>(4)), read<i256b>(%1))));
// DEFAULT-NEXT:         write<f32>(%0, read<f32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
