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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Element = i32;
// IR-NEXT:     type @type1 Pair = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     fn %2 @pointers(%3 p: ptr<i32>, %4 q: ptr<const i32>, %5 n: u64, %6 r: ptr<@type1>, %7 rows: ptr<array<i32, 3>>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%3), read<u64>(%5));
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%3), read<u64>(%5));
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=true>(read<ptr<i32>>(%3), read<u64>(%5));
// IR-NEXT:         let %8: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// IR-NEXT:         let %9: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%8), read<u64>(%5));
// IR-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%9));
// IR-NEXT:         let %10: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// IR-NEXT:         let %11: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true>(read<ptr<i32>>(%10), read<u64>(%5));
// IR-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%11));
// IR-NEXT:         let %12: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// IR-NEXT:         let %13: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%12), const<i32>(1));
// IR-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%13));
// IR-NEXT:         let %14: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// IR-NEXT:         let %15: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true>(read<ptr<i32>>(%14), const<i32>(1));
// IR-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%15));
// IR-NEXT:         ptr_offset<ptr<@type1>, subtract=false>(read<ptr<@type1>>(%6), read<u64>(%5));
// IR-NEXT:         ptr_offset<ptr<array<i32, 3>>, subtract=false>(read<ptr<array<i32, 3>>>(%7), const<i32>(1));
// IR-NEXT:         ptr_diff<i64>(read<ptr<@type1>>(%6), read<ptr<@type1>>(%6));
// IR-NEXT:         ptr_diff<i64>(read<ptr<array<i32, 3>>>(%7), read<ptr<array<i32, 3>>>(%7));
// IR-NEXT:         let %16: u64 [synthetic] = read<u64>(%5);
// IR-NEXT:         let %17: u64 [synthetic] = add<u64>(read<u64>(%16), reinterpret<u64>(widen<i64>(const<i32>(1))));
// IR-NEXT:         write<u64>(%5, read<u64>(%17));
// IR-NEXT:         let %18: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false>(read<ptr<i32>>(%3), read<u64>(%16));
// IR-NEXT:         let %19: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%18)));
// IR-NEXT:         let %20: i32 [synthetic] = add<i32>(read<i32>(%19), const<i32>(1));
// IR-NEXT:         write<i32>(deref(read<ptr<i32>>(%18)), read<i32>(%20));
// IR-NEXT:         return ptr_diff<i64>(read<ptr<i32>>(%3), read<ptr<const i32>>(%4));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
