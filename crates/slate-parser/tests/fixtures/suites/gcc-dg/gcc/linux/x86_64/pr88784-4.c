/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#include <limits.h>

_Bool and1(unsigned x, unsigned y)
{
  /* x > y && x == 0 --> false */
  return x > y && x == 0;
}

_Bool and2(unsigned x, unsigned y)
{
  /* x < y && x == UINT_MAX --> false */
  return x < y && x == UINT_MAX;
}

_Bool and3(signed x, signed y)
{
  /* x > y && x == INT_MIN --> false */
  return x > y && x == INT_MIN;
}

_Bool and4(signed x, signed y)
{
  /* x < y && x == INT_MAX --> false */
  return x < y && x == INT_MAX;
}

/* { dg-final { scan-tree-dump-not " == " "optimized" } } */
/* { dg-final { scan-tree-dump-not " > " "optimized" } } */
/* { dg-final { scan-tree-dump-not " < " "optimized" } } */

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
// DEFAULT-NEXT:     fn %[[VALUE_and1:[0-9]+]] @and1(%[[VALUE_x:[0-9]+]] x: u32, %[[VALUE_y:[0-9]+]] y: u32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_and<bool>(gt<u32>(read<u32>(%[[VALUE_x]]), read<u32>(%[[VALUE_y]])), eq<u32>(read<u32>(%[[VALUE_x]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_and2:[0-9]+]] @and2(%[[VALUE_x_2:[0-9]+]] x: u32, %[[VALUE_y_2:[0-9]+]] y: u32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_and<bool>(lt<u32>(read<u32>(%[[VALUE_x_2]]), read<u32>(%[[VALUE_y_2]])), eq<u32>(read<u32>(%[[VALUE_x_2]]), add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_and3:[0-9]+]] @and3(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_y_3]])), eq<i32>(read<i32>(%[[VALUE_x_3]]), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_and4:[0-9]+]] @and4(%[[VALUE_x_4:[0-9]+]] x: i32, %[[VALUE_y_4:[0-9]+]] y: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_x_4]]), read<i32>(%[[VALUE_y_4]])), eq<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(2147483647)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
