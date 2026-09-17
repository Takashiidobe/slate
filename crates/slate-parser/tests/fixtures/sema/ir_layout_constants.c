// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

struct Inner { char c; int value; };
struct Outer { char tag; struct Inner inner; int items[3]; };
unsigned long layout(int x) {
    sizeof(int);
    _Alignof(double);
    sizeof(struct Outer);
    sizeof(x++);
    sizeof("abc");
    sizeof(int[sizeof(short) + 1]);
    __builtin_offsetof(struct Outer, inner.value);
    return __builtin_offsetof(struct Outer, items[2]);
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
// IR-NEXT:     type @type0 Inner = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 value: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 Outer = struct {
// IR-NEXT:         field0 tag: i8;
// IR-NEXT:         field1 inner: @type0;
// IR-NEXT:         field2 items: array<i32, 3>;
// IR-NEXT:     } [size=24, align=4, offsets=[0, 4, 12]];
// IR-NEXT:     fn %2 @layout(%3 x: i32 [c="int"]) -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long"] [c="unsigned long(int)"] {
// IR-NEXT:         const<u64>(4) [size_of="i32"];
// IR-NEXT:         const<u64>(8) [align_of="f64"];
// IR-NEXT:         const<u64>(24) [size_of="@type1"];
// IR-NEXT:         const<u64>(4) [size_of="i32"];
// IR-NEXT:         const<u64>(4) [size_of="array<i8, 4>"];
// IR-NEXT:         const<u64>(12) [size_of="array<i32, 3>"];
// IR-NEXT:         const<u64>(8) [offset_of="@type1.inner.value"];
// IR-NEXT:         return const<u64>(20) [offset_of="@type1.items[2]"];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
