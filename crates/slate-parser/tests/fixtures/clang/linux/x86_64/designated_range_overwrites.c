// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

struct P { int x, y; };
struct I { int j; int k[3]; int l; };
struct M { int n; struct I o[3]; int p; };

int overlap[3][3] = {[0 ... 1] = {[1 ... 2] = 23}, [1][2] = 24};
int partial[][2][4] = {[2 ... 4][0 ... 1][2 ... 3] = 1, [2] = 2, [2][0][2] = 3};
int elided_range[3][2] = {[0 ... 1] = 2, 3};
int merged_range[4][3] = {[1][2] = 9, [0 ... 2] = 2, 3};
int walk_out[3][2] = {[1][0] = 1, 2, 3};

struct P pair = {.y = 1};
struct M deep[] = {[0 ... 5].o[1 ... 2].k[0 ... 1] = 4, 5, 6, 7};
struct M braced[] = {[0 ... 5].o = {[1 ... 2].k[0 ... 1] = 4}, [5].o[2].k[2] = 5, 6, 7};
union U { int a; short b; };
union U reopened[3] = {[1].b = 5, [0 ... 2] = 7};
struct P elided_pairs[3] = {[0 ... 2] = 1, 2};

_Static_assert(sizeof(deep) / sizeof(deep[0]) == 6);
_Static_assert(sizeof(partial) / sizeof(partial[0]) == 5);

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
// IR-NEXT:     type @type[[TYPE_I:[0-9]+]] I = struct {
// IR-NEXT:         field0 j: i32;
// IR-NEXT:         field1 k: array<i32, 3>;
// IR-NEXT:         field2 l: i32;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4, 16]];
// IR-NEXT:     type @type[[TYPE_M:[0-9]+]] M = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 o: array<@type[[TYPE_I]], 3>;
// IR-NEXT:         field2 p: i32;
// IR-NEXT:     } [size=68, align=4, offsets=[0, 4, 64]];
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i16;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     global %[[VALUE_overlap:[0-9]+]] overlap: array<array<i32, 3>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 3>, 3>, zero_fill=true>(index0 = aggregate<array<i32, 3>, zero_fill=true>(index1..=2 = const<i32>(23)), index1 = aggregate<array<i32, 3>, zero_fill=true>(index1 = const<i32>(23), index2 = const<i32>(24))) [linkage=external];
// IR-NEXT:     global %[[VALUE_partial:[0-9]+]] partial: array<array<array<i32, 4>, 2>, 5> [storage=static] [align=16] = aggregate<array<array<array<i32, 4>, 2>, 5>, zero_fill=true>(index2 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(1)), index1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1))), index3..=4 = aggregate<array<array<i32, 4>, 2>, zero_fill=false>(index0..=1 = aggregate<array<i32, 4>, zero_fill=true>(index2..=3 = const<i32>(1)))) [linkage=external];
// IR-NEXT:     global %[[VALUE_elided_range:[0-9]+]] elided_range: array<array<i32, 2>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 3>, zero_fill=true>(index0..=1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %[[VALUE_merged_range:[0-9]+]] merged_range: array<array<i32, 3>, 4> [storage=static] [align=16] = aggregate<array<array<i32, 3>, 4>, zero_fill=true>(index0 = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(2), index1 = const<i32>(3)), index1 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3), index2 = const<i32>(9)), index2 = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(2), index1 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %[[VALUE_walk_out:[0-9]+]] walk_out: array<array<i32, 2>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 3>, zero_fill=true>(index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index2 = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %[[VALUE_pair:[0-9]+]] pair: @type[[TYPE_P]] [storage=static] = aggregate<@type[[TYPE_P]], zero_fill=true>(field1 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %[[VALUE_deep:[0-9]+]] deep: array<@type[[TYPE_M]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_M]], 6>, zero_fill=false>(index0..=4 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1..=2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// IR-NEXT:     global %[[VALUE_braced:[0-9]+]] braced: array<@type[[TYPE_M]], 6> [storage=static] [align=16] = aggregate<array<@type[[TYPE_M]], 6>, zero_fill=false>(index0..=4 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1..=2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))))), index5 = aggregate<@type[[TYPE_M]], zero_fill=true>(field1 = aggregate<array<@type[[TYPE_I]], 3>, zero_fill=true>(index1 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=true>(index0..=1 = const<i32>(4))), index2 = aggregate<@type[[TYPE_I]], zero_fill=true>(field1 = aggregate<array<i32, 3>, zero_fill=false>(index0..=1 = const<i32>(4), index2 = const<i32>(5)), field2 = const<i32>(6))), field2 = const<i32>(7))) [linkage=external];
// IR-NEXT:     global %[[VALUE_reopened:[0-9]+]] reopened: array<@type[[TYPE_U]], 3> [storage=static] = aggregate<array<@type[[TYPE_U]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = const<i32>(7)), index1 = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = const<i32>(7)), index2 = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = const<i32>(7))) [linkage=external];
// IR-NEXT:     global %[[VALUE_elided_pairs:[0-9]+]] elided_pairs: array<@type[[TYPE_P]], 3> [storage=static] [align=16] = aggregate<array<@type[[TYPE_P]], 3>, zero_fill=false>(index0..=2 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2))) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
