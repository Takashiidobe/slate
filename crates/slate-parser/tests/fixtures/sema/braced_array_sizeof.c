// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct P { int x; int y; };
int plain[] = {1, 2};
int designated[] = {[3] = 1, 2};
struct P elided[] = {1, 2, 3, 4, 5};
struct P nested[] = {{1, 2}, {3, 4}};
int rows[][2] = {1, 2, 3};
static_assert(sizeof(plain) == 8);
static_assert(sizeof(designated) == 20);
static_assert(sizeof(elided) == 24);
static_assert(sizeof(nested) == 16);
static_assert(sizeof(rows) == 16);

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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %1 plain: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %2 designated: array<i32, 5> [storage=static] = aggregate<array<i32, 5>, zero_fill=true>(index3 = const<i32>(1), index4 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %3 elided: array<@type0, 3> [storage=static] = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), index2 = aggregate<@type0, zero_fill=true>(field0 = const<i32>(5))) [linkage=external];
// IR-NEXT:     global %4 nested: array<@type0, 2> [storage=static] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %5 rows: array<array<i32, 2>, 2> [storage=static] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(3))) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
