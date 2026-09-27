/* PR tree-optimization/113120 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

_BitInt(8) a;
_BitInt(55) b;

#if __BITINT_MAXWIDTH__ >= 401
static __attribute__((noinline, noclone)) void
foo (unsigned _BitInt(1) c, _BitInt(401) d)
{
  c /= d << b;
  a = c;
}

void
bar (void)
{
  foo (1, 4);
}
#endif

#if __BITINT_MAXWIDTH__ >= 6928
_BitInt(6928)
baz (int x, _BitInt(6928) y)
{
  if (x)
    return y;
  else
    return 0;
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
// DEFAULT-NEXT:     global %0 a: i8b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i55b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 c: u1b, %4 d: i401b) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9: u1b [synthetic] = read<u1b>(%3);
// DEFAULT-NEXT:         let %10: u1b [synthetic] = reinterpret<u1b, reason=assign, fits=unknown>(truncate<i1b, reason=assign, fits=unknown>(div<i401b, by_zero=ub, min_by_neg_one=ub>(reinterpret<i401b, reason=usual_arith, fits=unknown>(widen<u401b, reason=usual_arith>(read<u1b>(%9))), shl<i401b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i401b>(%4), read<i55b>(%1)))));
// DEFAULT-NEXT:         write<u1b>(%3, read<u1b>(%10));
// DEFAULT-NEXT:         write<i8b>(%0, reinterpret<i8b, reason=assign, fits=unknown>(widen<u8b, reason=assign>(read<u1b>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(u1b, i401b) -> void>(%2, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(const<i32>(1))), widen<i401b, reason=arg>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 x: i32, %8 y: i6928b) -> i6928b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             return read<i6928b>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return widen<i6928b, reason=return>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
