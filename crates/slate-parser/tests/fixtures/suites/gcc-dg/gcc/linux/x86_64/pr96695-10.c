/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#include <limits.h>

_Bool or1(unsigned *x, unsigned *y)
{
  /* x <= y || x != 0 --> true */
  return x <= y || x != 0;
}

_Bool or2(unsigned *x, unsigned *y)
{
  /* x >= y || x != -1 --> true */
  return x >= y || x != (unsigned*)-1;
}

/* { dg-final { scan-tree-dump-not " != " "optimized" } } */
/* { dg-final { scan-tree-dump-not " <= " "optimized" } } */
/* { dg-final { scan-tree-dump-not " >= " "optimized" } } */

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
// DEFAULT-NEXT:     fn %0 @or1(%1 x: ptr<u32>, %2 y: ptr<u32>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_or<bool>(le<ptr<u32>>(read<ptr<u32>>(%1), read<ptr<u32>>(%2)), ne<ptr<u32>>(read<ptr<u32>>(%1), null<ptr<u32>>));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @or2(%4 x: ptr<u32>, %5 y: ptr<u32>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_or<bool>(ge<ptr<u32>>(read<ptr<u32>>(%4), read<ptr<u32>>(%5)), ne<ptr<u32>>(read<ptr<u32>>(%4), int_to_ptr<ptr<u32>, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
