/* Verify that the 2 constant initializers are uniquized.  */

/* { dg-do compile } */
/* { dg-options "-Os -fdump-tree-gimple" } */

int lookup1 (int i)
{
  /* We use vectors long enough that piece-wise initialization is not
     reasonably preferable even for size (when including the constant
     vectors for initialization) for any target.  */
  int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
  return a[i];
}

int lookup2 (int i)
{
  int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
  return a[i+1];
}

/* { dg-final { scan-tree-dump-times "\[lL\]\\\$?C\[.:\]*0" 2 "gimple" } } */

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
// DEFAULT-NEXT:     fn %0 @lookup1(%1 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 a: array<i32, 32> [storage=automatic] [align=16] = aggregate<array<i32, 32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5), index6 = const<i32>(6), index7 = const<i32>(7), index8 = const<i32>(8), index9 = const<i32>(9), index10 = const<i32>(10), index11 = const<i32>(11), index12 = const<i32>(12), index13 = const<i32>(13), index14 = const<i32>(14), index15 = const<i32>(15), index16 = const<i32>(16), index17 = const<i32>(17), index18 = const<i32>(18), index19 = const<i32>(19), index20 = const<i32>(20), index21 = const<i32>(21), index22 = const<i32>(22), index23 = const<i32>(23), index24 = const<i32>(24), index25 = const<i32>(25), index26 = const<i32>(26), index27 = const<i32>(27), index28 = const<i32>(28), index29 = const<i32>(29), index30 = const<i32>(30), index31 = const<i32>(31));
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(32)>(%2), read<i32>(%1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @lookup2(%4 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 a: array<i32, 32> [storage=automatic] [align=16] = aggregate<array<i32, 32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(4), index5 = const<i32>(5), index6 = const<i32>(6), index7 = const<i32>(7), index8 = const<i32>(8), index9 = const<i32>(9), index10 = const<i32>(10), index11 = const<i32>(11), index12 = const<i32>(12), index13 = const<i32>(13), index14 = const<i32>(14), index15 = const<i32>(15), index16 = const<i32>(16), index17 = const<i32>(17), index18 = const<i32>(18), index19 = const<i32>(19), index20 = const<i32>(20), index21 = const<i32>(21), index22 = const<i32>(22), index23 = const<i32>(23), index24 = const<i32>(24), index25 = const<i32>(25), index26 = const<i32>(26), index27 = const<i32>(27), index28 = const<i32>(28), index29 = const<i32>(29), index30 = const<i32>(30), index31 = const<i32>(31));
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(32)>(%5), add<i32, overflow=ub>(read<i32>(%4), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
