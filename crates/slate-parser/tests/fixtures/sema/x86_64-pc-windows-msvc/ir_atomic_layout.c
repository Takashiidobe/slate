// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct odd3 { char a[3]; };
struct odd5 { char a[5]; };
struct odd9 { char a[9]; };
struct wide16 { char a[16]; };
struct wide17 { char a[17]; };
struct aligned { double d; int i; };

// MSVC does not promote to the next power of two the way clang does: a value
// that is already a lock-free width keeps its size and is aligned to it, and
// anything else gets a leading four-byte lock, so it lays out like
// struct { int lock; T value; }.
unsigned long records[] = {
    sizeof(_Atomic struct odd3),   _Alignof(_Atomic struct odd3),
    sizeof(_Atomic struct odd5),   _Alignof(_Atomic struct odd5),
    sizeof(_Atomic struct odd9),   _Alignof(_Atomic struct odd9),
    sizeof(_Atomic struct wide16), _Alignof(_Atomic struct wide16),
    sizeof(_Atomic struct wide17), _Alignof(_Atomic struct wide17),
    sizeof(_Atomic struct aligned), _Alignof(_Atomic struct aligned),
};

unsigned long scalars[] = {
    sizeof(_Atomic char),        _Alignof(_Atomic char),
    sizeof(_Atomic short),       _Alignof(_Atomic short),
    sizeof(_Atomic int),         _Alignof(_Atomic int),
    sizeof(_Atomic long long),   _Alignof(_Atomic long long),
    sizeof(_Atomic double),      _Alignof(_Atomic double),
    sizeof(_Atomic long double), _Alignof(_Atomic long double),
    sizeof(_Atomic char *),      _Alignof(_Atomic char *),
};

// the lock leads the object, so it reaches member offsets too
struct member { char head; _Atomic struct odd3 value; char tail; };
struct elements { _Atomic struct odd3 values[3]; };

_Static_assert(sizeof(_Atomic struct odd3) == 8, "lock plus value");
_Static_assert(_Alignof(_Atomic struct odd3) == 4, "lock alignment");
_Static_assert(sizeof(struct member) == 16, "member layout");
_Static_assert(__builtin_offsetof(struct member, value) == 4, "member offset");
_Static_assert(__builtin_offsetof(struct member, tail) == 12, "tail offset");
_Static_assert(sizeof(struct elements) == 24, "per element lock");

_Atomic struct odd3 object;

unsigned long objects(_Atomic struct odd3 *pointer, struct member *record) {
    return sizeof(object) + _Alignof(object) + sizeof(*pointer) + sizeof(record->value);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 odd3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type1 odd5 = struct {
// IR-NEXT:         field0 a: array<i8, 5>;
// IR-NEXT:     } [size=5, align=1, offsets=[0]];
// IR-NEXT:     type @type2 odd9 = struct {
// IR-NEXT:         field0 a: array<i8, 9>;
// IR-NEXT:     } [size=9, align=1, offsets=[0]];
// IR-NEXT:     type @type3 wide16 = struct {
// IR-NEXT:         field0 a: array<i8, 16>;
// IR-NEXT:     } [size=16, align=1, offsets=[0]];
// IR-NEXT:     type @type4 wide17 = struct {
// IR-NEXT:         field0 a: array<i8, 17>;
// IR-NEXT:     } [size=17, align=1, offsets=[0]];
// IR-NEXT:     type @type5 aligned = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type6 member = struct {
// IR-NEXT:         field0 head: i8;
// IR-NEXT:         field1 value: atomic @type0;
// IR-NEXT:         field2 tail: i8;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 4, 12]];
// IR-NEXT:     type @type7 elements = struct {
// IR-NEXT:         field0 values: atomic array<@type0, 3>;
// IR-NEXT:     } [size=24, align=4, offsets=[0]];
// IR-NEXT:     global %6 records: array<u32, 12> [storage=static] [align=16] = aggregate<array<u32, 12>, zero_fill=false>(index0 = truncate<u32>(const<u64>(8)), index1 = truncate<u32>(const<u64>(4)), index2 = truncate<u32>(const<u64>(12)), index3 = truncate<u32>(const<u64>(4)), index4 = truncate<u32>(const<u64>(16)), index5 = truncate<u32>(const<u64>(4)), index6 = truncate<u32>(const<u64>(20)), index7 = truncate<u32>(const<u64>(4)), index8 = truncate<u32>(const<u64>(24)), index9 = truncate<u32>(const<u64>(4)), index10 = truncate<u32>(const<u64>(24)), index11 = truncate<u32>(const<u64>(8))) [linkage=external];
// IR-NEXT:     global %7 scalars: array<u32, 14> [storage=static] [align=16] = aggregate<array<u32, 14>, zero_fill=false>(index0 = truncate<u32>(const<u64>(1)), index1 = truncate<u32>(const<u64>(1)), index2 = truncate<u32>(const<u64>(2)), index3 = truncate<u32>(const<u64>(2)), index4 = truncate<u32>(const<u64>(4)), index5 = truncate<u32>(const<u64>(4)), index6 = truncate<u32>(const<u64>(8)), index7 = truncate<u32>(const<u64>(8)), index8 = truncate<u32>(const<u64>(8)), index9 = truncate<u32>(const<u64>(8)), index10 = truncate<u32>(const<u64>(8)), index11 = truncate<u32>(const<u64>(8)), index12 = truncate<u32>(const<u64>(8)), index13 = truncate<u32>(const<u64>(8))) [linkage=external];
// IR-NEXT:     global %10 object: atomic @type0 [storage=static] [linkage=external];
// IR-NEXT:     fn %11 @objects(%12 pointer: ptr<atomic @type0>, %13 record: ptr<@type6>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return truncate<u32>(add<u64>(add<u64>(add<u64>(const<u64>(8), const<u64>(4)), const<u64>(8)), const<u64>(8)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
