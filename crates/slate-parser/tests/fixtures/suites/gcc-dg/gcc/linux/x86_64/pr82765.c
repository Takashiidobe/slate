/* { dg-do compile } */
/* { dg-options "-fpermissive -Os -w" } */

int a[1][1];
int main() { int *b[] = {a, a[1820408606019012862278468], a, a, a}; }

// SLATE-FILECHECK-ARGS -fpermissive
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<array<i32, 1>, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<ptr<i32>, 5> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 5>, zero_fill=false>(index0 = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(%[[VALUE_a]])), index1 = array_decay<ptr<i32>, length=Some(1)>(deref(ptr_offset<ptr<array<i32, 1>>, subtract=false, element=array<i32, 1>, overflow=ub>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(%[[VALUE_a]]), const<i128>(10113849059470605124)))), index2 = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(%[[VALUE_a]])), index3 = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(%[[VALUE_a]])), index4 = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
