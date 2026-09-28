// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

char grid[2][3];
int cube[2][3][4];
int *pointers[2][3];
int (*row_pointer)[2][3];
int open_rows[][2] = {{1, 2}, {3, 4}, {5, 6}};

static_assert(sizeof(grid) == 6);
static_assert(sizeof(cube) == 96);
static_assert(sizeof(pointers) == 48);

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
// IR-NEXT:     global %0 grid: array<array<i8, 3>, 2> [storage=static] [linkage=external];
// IR-NEXT:     global %1 cube: array<array<array<i32, 4>, 3>, 2> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %2 pointers: array<array<ptr<i32>, 3>, 2> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %3 row_pointer: ptr<array<array<i32, 3>, 2>> [storage=static] [linkage=external];
// IR-NEXT:     global %4 open_rows: array<array<i32, 2>, 3> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 3>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4)), index2 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(6))) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
