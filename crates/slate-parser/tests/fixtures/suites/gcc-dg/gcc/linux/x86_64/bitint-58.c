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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i3b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i8b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i495b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i513b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i1085b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i4096b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i3b [synthetic] = read<i3b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i3b [synthetic] = truncate<i3b, reason=assign, fits=unknown>(sub<i4097b, overflow=ub>(widen<i4097b, reason=usual_arith>(read<i3b>(%[[VALUE0]])), shr<i4097b, amount_out_of_range=ub, fill=sign_extend>(widen<i4097b, reason=explicit>(read<i513b>(%[[VALUE_d]])), read<i8b>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         write<i3b>(%[[VALUE_a]], read<i3b>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         overflow_sub<bool>(shr<i767b, amount_out_of_range=ub, fill=sign_extend>(widen<i767b, reason=explicit>(read<i495b>(%[[VALUE_c]])), read<i1085b>(%[[VALUE_e]])), const<i32>(0), deref(addr_of<ptr<i3b>>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i768b [storage=automatic] = widen<i768b, reason=assign>(truncate<i257b, reason=explicit, fits=unknown>(read<i4096b>(%[[VALUE_f]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i8b [synthetic] = read<i8b>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i8b [synthetic] = truncate<i8b, reason=assign, fits=unknown>(div<i768b, by_zero=ub, min_by_neg_one=ub>(widen<i768b, reason=usual_arith>(read<i8b>(%[[VALUE2]])), shr<i768b, amount_out_of_range=ub, fill=sign_extend>(read<i768b>(%[[VALUE_x]]), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(0), const<i32>(0)))));
// DEFAULT-NEXT:         write<i8b>(%[[VALUE_b]], read<i8b>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
