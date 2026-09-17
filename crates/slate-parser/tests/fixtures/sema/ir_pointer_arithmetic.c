// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

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
// IR-NEXT:     fn %2 @pointers(%3 p: ptr<i32>, %4 q: ptr<const i32>, %5 n: u64, %6 r: ptr<@type1>, %7 rows: ptr<array<i32, 3>>) -> i64 [linkage=external] {
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<u64>(%5));
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<u64>(%5));
// IR-NEXT:         ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<u64>(%5));
// IR-NEXT:         update<ptr<i32>, result=new>(%3, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, read<u64>(%5)));
// IR-NEXT:         update<ptr<i32>, result=new>(%3, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, read<u64>(%5)));
// IR-NEXT:         update<ptr<i32>, result=old>(%3, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// IR-NEXT:         update<ptr<i32>, result=new>(%3, ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(old<ptr<i32>>, const<i32>(1)));
// IR-NEXT:         ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(read<ptr<@type1>>(%6), read<u64>(%5));
// IR-NEXT:         ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(read<ptr<array<i32, 3>>>(%7), const<i32>(1));
// IR-NEXT:         ptr_diff<i64, element=@type1, same_array=required, overflow=ub>(read<ptr<@type1>>(%6), read<ptr<@type1>>(%6));
// IR-NEXT:         ptr_diff<i64, element=array<i32, 3>, same_array=required, overflow=ub>(read<ptr<array<i32, 3>>>(%7), read<ptr<array<i32, 3>>>(%7));
// IR-NEXT:         update<i32, result=new>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), update<u64, result=old>(%5, add<u64, overflow=wrap>(old<u64>, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))), add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// IR-NEXT:         return ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%3), read<ptr<const i32>>(%4));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
