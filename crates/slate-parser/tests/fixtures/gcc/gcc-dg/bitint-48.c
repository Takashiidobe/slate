/* PR tree-optimization/112809 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 512
_BitInt (512) a;
_BitInt (256) b;
_BitInt (256) c;

int
foo (void)
{
  return a == (b | c);
}

void
bar (void)
{
  a /= b - 2;
}
#else
int i;
#endif

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 a: i512b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i256b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i256b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i512b>(read<i512b>(%0), widen<i512b, reason=usual_arith>(or<i256b>(read<i256b>(%1), read<i256b>(%2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5: i512b [synthetic] = read<i512b>(%0);
// DEFAULT-NEXT:         let %6: i512b [synthetic] = div<i512b, by_zero=ub, min_by_neg_one=ub>(read<i512b>(%5), widen<i512b, reason=usual_arith>(sub<i256b, overflow=ub>(read<i256b>(%1), widen<i256b, reason=usual_arith>(const<i32>(2)))));
// DEFAULT-NEXT:         write<i512b>(%0, read<i512b>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
