/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-original -fdump-tree-optimized" } */
/* { dg-add-options ieee } */
/* PR middle-end/95351 */

int Foo(double possiblyNAN, double b, double c)
{
    return (possiblyNAN <= 2.0) || ((possiblyNAN  > 2.0) && (b > c));
}

/* Make sure we don't remove either >/<=  */

/* { dg-final { scan-tree-dump "possiblyNAN > 2.0e.0" "original" } } */
/* { dg-final { scan-tree-dump "possiblyNAN_\[0-9\]+.D. > 2.0e.0" "optimized" } } */

/* { dg-final { scan-tree-dump "possiblyNAN <= 2.0e.0" "original" } } */
/* { dg-final { scan-tree-dump "possiblyNAN_\[0-9\]+.D. <= 2.0e.0" "optimized" } } */

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
// DEFAULT-NEXT:     fn %[[VALUE_Foo:[0-9]+]] @Foo(%[[VALUE_possiblyNAN:[0-9]+]] possiblyNAN: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(le<f64, exceptions=ignore>(read<f64>(%[[VALUE_possiblyNAN]]), const<f64>(2.0)), logical_and<bool>(gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_possiblyNAN]]), const<f64>(2.0)), gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_b]]), read<f64>(%[[VALUE_c]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
