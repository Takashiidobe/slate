/* { dg-do compile } */
/* { dg-options "-O2 -fdisable-tree-evrp -fdump-tree-vrp1 -fno-tree-ccp" } */

#include <limits.h>

/* Return x so that its range is still exported after the comparison folds
   away.  (x & LLONG_MIN) < 1 is true for both values x can take, so the
   test of it no longer keeps x live by itself.  */

long long foo (void)
{
    volatile int a = -1;
    long long b = LLONG_MIN;
    long long x = (a & b); // x == 0x8000000000000000
    if (x < 1LL) { ; } else { __builtin_abort(); }
    return x;
}

/* { dg-final { scan-tree-dump "\\\[-INF, -INF\\\]\\\[0, 0\\\]" "vrp1" } } */

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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: volatile i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i64 [storage=automatic] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1));
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i64 [storage=automatic] = and<i64>(widen<i64, reason=usual_arith>(read<i32, volatile>(%[[VALUE_a]])), read<i64>(%[[VALUE_b]]));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_x]]), const<i64>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
