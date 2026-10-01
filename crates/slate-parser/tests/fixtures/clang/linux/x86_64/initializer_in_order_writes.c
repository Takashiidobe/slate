// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

struct P { int x, y; };

int back_then_forward[6] = {1, 2, [1] = 7, 3, 4};
int rewind_inside[5] = {1, 2, 3, [1] = 8, 9};
int range_after_tail[8] = {1, 2, [1 ... 4] = 5, 6};
struct P reopened[3] = {{1, 2}, [0].y = 5, {3, 4}, [1].x = 6};
struct P member_runs[3] = {[0].x = 1, [0].y = 2, [1].x = 3, [2].y = 4, [1].y = 5};
int elided_rows[4][2] = {1, 2, 3, [1][1] = 9, 4};

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %[[VALUE_back_then_forward:[0-9]+]] back_then_forward: array<i32, 6> [storage=static] [align=16] = aggregate<array<i32, 6>, zero_fill=true>(index0 = const<i32>(1), index1 = const<i32>(7), index2 = const<i32>(3), index3 = const<i32>(4)) [linkage=external];
// IR-NEXT:     global %[[VALUE_rewind_inside:[0-9]+]] rewind_inside: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=true>(index0 = const<i32>(1), index1 = const<i32>(8), index2 = const<i32>(9)) [linkage=external];
// IR-NEXT:     global %[[VALUE_range_after_tail:[0-9]+]] range_after_tail: array<i32, 8> [storage=static] [align=16] = aggregate<array<i32, 8>, zero_fill=true>(index0 = const<i32>(1), index1..=4 = const<i32>(5), index5 = const<i32>(6)) [linkage=external];
// IR-NEXT:     global %[[VALUE_reopened:[0-9]+]] reopened: array<@type[[TYPE_P]], 3> [storage=static] [align=16] = aggregate<array<@type[[TYPE_P]], 3>, zero_fill=true>(index0 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(5)), index1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_member_runs:[0-9]+]] member_runs: array<@type[[TYPE_P]], 3> [storage=static] [align=16] = aggregate<array<@type[[TYPE_P]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(5)), index2 = aggregate<@type[[TYPE_P]], zero_fill=true>(field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_elided_rows:[0-9]+]] elided_rows: array<array<i32, 2>, 4> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 4>, zero_fill=true>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(9)), index2 = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(4))) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
