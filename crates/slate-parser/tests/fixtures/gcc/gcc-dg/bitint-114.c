/* PR middle-end/117571 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 255
_BitInt(255) b;

_BitInt(255)
foo ()
{
  return (b << 10) / 2;
}
#endif

#if __BITINT_MAXWIDTH__ >= 8192
_BitInt(8192) c;

_BitInt(8192)
bar ()
{
  return (c << 1039) / 0x20000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb;
}
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
// DEFAULT-NEXT:     global %0 b: i255b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i8192b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo() -> i255b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i255b, by_zero=ub, min_by_neg_one=ub>(shl<i255b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i255b>(%0), const<i32>(10)), widen<i255b, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar() -> i8192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i8192b, by_zero=ub, min_by_neg_one=ub>(shl<i8192b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i8192b>(%2), const<i32>(1039)), widen<i8192b, reason=usual_arith>(const<i1039b>(1472670216079209191611846812294369061779846741149537544383939224844146080198663889983147846225162535085015972903906454385940805786127700971461406151798572026902674582936498055383467782973408003026559655543480367258322130389749455925034296201550456726842167383528130955181647838728025835969211239052281644132073472)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
