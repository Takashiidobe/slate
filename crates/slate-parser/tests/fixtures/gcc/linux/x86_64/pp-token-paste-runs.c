#define fifty int fif ## ty
#define object_paste fo ## o
#define run_object a ## ## b
#define cat(x, y) x ## ## y
#define long_run(x, y) x##########y

int ab, abcd, cd, foo;
fifty = 50;
int *pasted[] = {&object_paste, &run_object, &cat(a, b), &long_run(ab, cd),
                 &long_run(, cd), &long_run(ab, )};

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
// DEFAULT-NEXT:     global %[[VALUE_ab:[0-9]+]] ab: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_abcd:[0-9]+]] abcd: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cd:[0-9]+]] cd: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fifty:[0-9]+]] fifty: i32 [storage=static] = const<i32>(50) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pasted:[0-9]+]] pasted: array<ptr<i32>, 6> [storage=static] [align=16] = aggregate<array<ptr<i32>, 6>, zero_fill=false>(index0 = addr_of<ptr<i32>>(%[[VALUE_foo]]), index1 = addr_of<ptr<i32>>(%[[VALUE_ab]]), index2 = addr_of<ptr<i32>>(%[[VALUE_ab]]), index3 = addr_of<ptr<i32>>(%[[VALUE_abcd]]), index4 = addr_of<ptr<i32>>(%[[VALUE_cd]]), index5 = addr_of<ptr<i32>>(%[[VALUE_ab]])) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
