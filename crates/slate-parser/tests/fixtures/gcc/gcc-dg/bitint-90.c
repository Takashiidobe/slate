/* PR tree-optimization/113567 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 129
_BitInt(129) v;

void
foo (_BitInt(129) a, int i)
{
  __label__  l1, l2;
  i &= 1;
  void *p[] = { &&l1, &&l2 };
l1:
  a %= 3;
  v = a;
  i = !i;
  goto *(p[i]);
l2:;
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
// DEFAULT-NEXT:     global %0 v: i129b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%4 a: i129b, %5 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %8: i32 [synthetic] = and<i32>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%8));
// DEFAULT-NEXT:         let %6 p: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%2), index1 = label_addr<ptr<void>>(%3));
// DEFAULT-NEXT:         label %2 l1:
// DEFAULT-NEXT:             let %9: i129b [synthetic] = read<i129b>(%4);
// DEFAULT-NEXT:             let %10: i129b [synthetic] = rem<i129b, by_zero=ub, min_by_neg_one=ub>(read<i129b>(%9), widen<i129b, reason=usual_arith>(const<i32>(3)));
// DEFAULT-NEXT:             write<i129b>(%4, read<i129b>(%10));
// DEFAULT-NEXT:         write<i129b>(%0, read<i129b>(%4));
// DEFAULT-NEXT:         write<i32>(%5, from_bool<i32, reason=assign>(not<bool>(ne<i32>(read<i32>(%5), const<i32>(0)))));
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%6), read<i32>(%5))));
// DEFAULT-NEXT:         label %3 l2:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
