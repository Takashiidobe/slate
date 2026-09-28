// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct odd5 { char a[5]; };
struct odd9 { char a[9]; };

// i686 promotes at most 8 bytes, so odd9 keeps its natural layout while
// long long is over-aligned past its natural 4.
unsigned long widths[] = {
    sizeof(_Atomic struct odd5), _Alignof(_Atomic struct odd5),
    sizeof(_Atomic struct odd9), _Alignof(_Atomic struct odd9),
    sizeof(_Atomic long long),   _Alignof(_Atomic long long),
    sizeof(long long),           _Alignof(long long),
};

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 odd5 = struct {
// IR-NEXT:         field0 a: array<i8, 5>;
// IR-NEXT:     } [size=5, align=1, offsets=[0]];
// IR-NEXT:     type @type1 odd9 = struct {
// IR-NEXT:         field0 a: array<i8, 9>;
// IR-NEXT:     } [size=9, align=1, offsets=[0]];
// IR-NEXT:     global %2 widths: array<u32, 8> [storage=static] = aggregate<array<u32, 8>, zero_fill=false>(index0 = const<u32>(8), index1 = const<u32>(8), index2 = const<u32>(9), index3 = const<u32>(1), index4 = const<u32>(8), index5 = const<u32>(8), index6 = const<u32>(8), index7 = const<u32>(4)) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
