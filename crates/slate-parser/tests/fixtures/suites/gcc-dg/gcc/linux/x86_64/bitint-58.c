/* PR tree-optimization/113102 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

_BitInt(3) a;
#if __BITINT_MAXWIDTH__ >= 4097
_BitInt(8) b;
_BitInt(495) c;
_BitInt(513) d;
_BitInt(1085) e;
_BitInt(4096) f;

void
foo (void)
{
  a -= (_BitInt(4097)) d >> b;
}

void
bar (void)
{
  __builtin_sub_overflow ((_BitInt(767)) c >> e, 0, &a);
}

void
baz (void)
{
  _BitInt(768) x = (_BitInt(257))f;
  b /= x >> 0 / 0;	/* { dg-warning "division by zero" } */
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
// DEFAULT-NEXT:     global %0 a: i3b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i8b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i495b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i1085b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i4096b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10: i3b [synthetic] = read<i3b>(%0);
// DEFAULT-NEXT:         let %11: i3b [synthetic] = truncate<i3b, reason=assign, fits=unknown>(sub<i4097b, overflow=ub>(widen<i4097b, reason=usual_arith>(read<i3b>(%10)), shr<i4097b, amount_out_of_range=ub, fill=sign_extend>(widen<i4097b, reason=explicit>(read<i513b>(%3)), read<i8b>(%1))));
// DEFAULT-NEXT:         write<i3b>(%0, read<i3b>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_sub<bool>(shr<i767b, amount_out_of_range=ub, fill=sign_extend>(widen<i767b, reason=explicit>(read<i495b>(%2)), read<i1085b>(%4)), const<i32>(0), deref(addr_of<ptr<i3b>>(%0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 x: i768b [storage=automatic] = widen<i768b, reason=assign>(truncate<i257b, reason=explicit, fits=unknown>(read<i4096b>(%5)));
// DEFAULT-NEXT:         let %12: i8b [synthetic] = read<i8b>(%1);
// DEFAULT-NEXT:         let %13: i8b [synthetic] = truncate<i8b, reason=assign, fits=unknown>(div<i768b, by_zero=ub, min_by_neg_one=ub>(widen<i768b, reason=usual_arith>(read<i8b>(%12)), shr<i768b, amount_out_of_range=ub, fill=sign_extend>(read<i768b>(%9), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)))));
// DEFAULT-NEXT:         write<i8b>(%1, read<i8b>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
