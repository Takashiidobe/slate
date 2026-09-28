/* PR tree-optimization/112734 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -fnon-call-exceptions -ftrapv" } */

#if __BITINT_MAXWIDTH__ >= 128
_BitInt(128) out;
#else
int out;
#endif

int
main ()
{
  _BitInt(8) q[1];
  out -= 1;
}

// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-ARGS -ftrapv
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
// DEFAULT-NEXT:     global %0 out: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 q: array<i8b, 1> [storage=automatic];
// DEFAULT-NEXT:         let %3: i128b [synthetic] = read<i128b>(%0);
// DEFAULT-NEXT:         let %4: i128b [synthetic] = sub<i128b, overflow=trap>(read<i128b>(%3), widen<i128b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128b>(%0, read<i128b>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
