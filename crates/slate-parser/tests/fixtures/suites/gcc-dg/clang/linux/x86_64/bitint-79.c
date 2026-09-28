/* PR tree-optimization/113639 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23" } */

int j, k;
#if __BITINT_MAXWIDTH__ >= 162
struct S { _BitInt(162) n; };
void bar (_BitInt(162) x);

void
foo (struct S s)
{
  bar (s.n * j);
  (void) (s.n * k);
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 n: i162b;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %0 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%7 x: i162b) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 s: @type0) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i162b) -> void>(%4, mul<i162b, overflow=ub>(read<i162b>(field0(%6)), widen<i162b, reason=usual_arith>(read<i32>(%0))));
// DEFAULT-NEXT:         mul<i162b, overflow=ub>(read<i162b>(field0(%6)), widen<i162b, reason=usual_arith>(read<i32>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
