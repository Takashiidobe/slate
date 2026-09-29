// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef int Element;
struct Pair { int x; long y; };
long pointers(Element *p, const int *q, unsigned long n, struct Pair *r, int (*rows)[3]) {
    p + n;
    n + p;
    p - n;
    p += n;
    p -= n;
    p++;
    --p;
    r + n;
    rows + 1;
    r - r;
    rows - rows;
    p[n++] += 1;
    return p - q;
}

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
// IR-NEXT:     type @type[[TYPE_Element:[0-9]+]] Element = i32;
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     fn %[[VALUE_pointers:[0-9]+]] @pointers(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_q:[0-9]+]] q: ptr<const i32>, %[[VALUE_n:[0-9]+]] n: u64, %[[VALUE_r:[0-9]+]] r: ptr<@type[[TYPE_Pair]]>, %[[VALUE_rows:[0-9]+]] rows: ptr<array<i32, 3>>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_p]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_p]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=true>(read<ptr<i32>>(%[[VALUE_p]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE0]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:         write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true>(read<ptr<i32>>(%[[VALUE2]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:         write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE4]]), const<i32>(1));
// IR-NEXT:         write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true>(read<ptr<i32>>(%[[VALUE6]]), const<i32>(1));
// IR-NEXT:         write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE7]]));
// IR-NEXT:         ptr_offset<ptr<@type[[TYPE_Pair]]>, subtract=false>(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_r]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:         ptr_offset<ptr<array<i32, 3>>, subtract=false>(read<ptr<array<i32, 3>>>(%[[VALUE_rows]]), const<i32>(1));
// IR-NEXT:         ptr_diff<i64>(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_r]]), read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_r]]));
// IR-NEXT:         ptr_diff<i64>(read<ptr<array<i32, 3>>>(%[[VALUE_rows]]), read<ptr<array<i32, 3>>>(%[[VALUE_rows]]));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_n]]);
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: u64 [synthetic] = add<u64>(read<u64>(%[[VALUE8]]), reinterpret<u64>(widen<i64>(const<i32>(1))));
// IR-NEXT:         write<u64>(%[[VALUE_n]], read<u64>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%[[VALUE_p]]), read<u64>(%[[VALUE8]]));
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE10]])));
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE11]]), const<i32>(1));
// IR-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE10]])), read<i32>(%[[VALUE12]]));
// IR-NEXT:         return ptr_diff<i64>(read<ptr<i32>>(%[[VALUE_p]]), read<ptr<const i32>>(%[[VALUE_q]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
