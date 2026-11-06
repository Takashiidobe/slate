// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct odd3 { char a[3]; };
struct odd5 { char a[5]; };
struct odd9 { char a[9]; };
struct wide17 { char a[17]; };

unsigned long records[] = {
    sizeof(_Atomic struct odd3),  _Alignof(_Atomic struct odd3),
    sizeof(_Atomic struct odd5),  _Alignof(_Atomic struct odd5),
    sizeof(_Atomic struct odd9),  _Alignof(_Atomic struct odd9),
    sizeof(_Atomic struct wide17), _Alignof(_Atomic struct wide17),
};

unsigned long scalars[] = {
    sizeof(_Atomic char),      _Alignof(_Atomic char),
    sizeof(_Atomic short),     _Alignof(_Atomic short),
    sizeof(_Atomic int),       _Alignof(_Atomic int),
    sizeof(_Atomic long long), _Alignof(_Atomic long long),
    sizeof(_Atomic double),    _Alignof(_Atomic double),
    sizeof(_Atomic char *),    _Alignof(_Atomic char *),
};

struct member { char head; _Atomic struct odd3 value; char tail; };
struct elements { _Atomic struct odd3 values[3]; };

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
// IR-NEXT:     type @type[[TYPE_odd3:[0-9]+]] odd3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_odd5:[0-9]+]] odd5 = struct {
// IR-NEXT:         field0 a: array<i8, 5>;
// IR-NEXT:     } [size=5, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_odd9:[0-9]+]] odd9 = struct {
// IR-NEXT:         field0 a: array<i8, 9>;
// IR-NEXT:     } [size=9, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_wide17:[0-9]+]] wide17 = struct {
// IR-NEXT:         field0 a: array<i8, 17>;
// IR-NEXT:     } [size=17, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_member:[0-9]+]] member = struct {
// IR-NEXT:         field0 head: i8;
// IR-NEXT:         field1 value: atomic @type[[TYPE_odd3]];
// IR-NEXT:         field2 tail: i8;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1, 4]];
// IR-NEXT:     type @type[[TYPE_elements:[0-9]+]] elements = struct {
// IR-NEXT:         field0 values: atomic array<@type[[TYPE_odd3]], 3>;
// IR-NEXT:     } [size=9, align=1, offsets=[0]];
// IR-NEXT:     global %[[VALUE_records:[0-9]+]] records: array<u64, 8> [storage=static] [align=16] = aggregate<array<u64, 8>, zero_fill=false>(index0 = reinterpret<u64, reason=assign>(const<u64>(3) [c="__size_t"]), index1 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"]), index2 = reinterpret<u64, reason=assign>(const<u64>(5) [c="__size_t"]), index3 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"]), index4 = reinterpret<u64, reason=assign>(const<u64>(9) [c="__size_t"]), index5 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"]), index6 = reinterpret<u64, reason=assign>(const<u64>(17) [c="__size_t"]), index7 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"])) [linkage=external];
// IR-NEXT:     global %[[VALUE_scalars:[0-9]+]] scalars: array<u64, 12> [storage=static] [align=16] = aggregate<array<u64, 12>, zero_fill=false>(index0 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"]), index1 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"]), index2 = reinterpret<u64, reason=assign>(const<u64>(2) [c="__size_t"]), index3 = reinterpret<u64, reason=assign>(const<u64>(2) [c="__size_t"]), index4 = reinterpret<u64, reason=assign>(const<u64>(4) [c="__size_t"]), index5 = reinterpret<u64, reason=assign>(const<u64>(4) [c="__size_t"]), index6 = reinterpret<u64, reason=assign>(const<u64>(8) [c="__size_t"]), index7 = reinterpret<u64, reason=assign>(const<u64>(8) [c="__size_t"]), index8 = reinterpret<u64, reason=assign>(const<u64>(8) [c="__size_t"]), index9 = reinterpret<u64, reason=assign>(const<u64>(8) [c="__size_t"]), index10 = reinterpret<u64, reason=assign>(size_of<u64, ptr<atomic i8>>(8) [c="__size_t"]), index11 = reinterpret<u64, reason=assign>(align_of<u64, ptr<atomic i8>>(8) [c="__size_t"])) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
