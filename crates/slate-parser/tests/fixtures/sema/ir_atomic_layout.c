// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct odd3 { char a[3]; };
struct odd5 { char a[5]; };
struct odd9 { char a[9]; };
struct wide17 { char a[17]; };

// _Atomic pads the value type up to a power of two and raises its alignment
// to match, but only up to the target's widest promotable width.
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
    sizeof(_Atomic long double), _Alignof(_Atomic long double),
    sizeof(_Atomic char *),    _Alignof(_Atomic char *),
};

// the qualifier lands on the element, so each element is promoted, not the array
struct member { char head; _Atomic struct odd3 value; char tail; };
struct elements { _Atomic struct odd3 values[3]; };
struct scalar_member { _Atomic int value; char tail; };

_Static_assert(sizeof(struct member) == 12, "member layout");
_Static_assert(__builtin_offsetof(struct member, value) == 4, "member offset");
_Static_assert(__builtin_offsetof(struct member, tail) == 8, "tail offset");
_Static_assert(sizeof(struct elements) == 12, "per element promotion");

_Atomic struct odd3 object;

unsigned long objects(_Atomic struct odd3 *pointer, struct member *record) {
    return sizeof(object) + _Alignof(object) + sizeof(*pointer) + sizeof(record->value);
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
// IR-NEXT:     type @type0 odd3 = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type1 odd5 = struct {
// IR-NEXT:         field0 a: array<i8, 5>;
// IR-NEXT:     } [size=5, align=1, offsets=[0]];
// IR-NEXT:     type @type2 odd9 = struct {
// IR-NEXT:         field0 a: array<i8, 9>;
// IR-NEXT:     } [size=9, align=1, offsets=[0]];
// IR-NEXT:     type @type3 wide17 = struct {
// IR-NEXT:         field0 a: array<i8, 17>;
// IR-NEXT:     } [size=17, align=1, offsets=[0]];
// IR-NEXT:     type @type4 member = struct {
// IR-NEXT:         field0 head: i8;
// IR-NEXT:         field1 value: atomic @type0;
// IR-NEXT:         field2 tail: i8;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type5 elements = struct {
// IR-NEXT:         field0 values: atomic array<@type0, 3>;
// IR-NEXT:     } [size=12, align=4, offsets=[0]];
// IR-NEXT:     type @type6 scalar_member = struct {
// IR-NEXT:         field0 value: atomic i32;
// IR-NEXT:         field1 tail: i8;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %4 records: array<u64, 8> [storage=static] = aggregate<array<u64, 8>, zero_fill=false>(index0 = const<u64>(4), index1 = const<u64>(4), index2 = const<u64>(8), index3 = const<u64>(8), index4 = const<u64>(16), index5 = const<u64>(16), index6 = const<u64>(17), index7 = const<u64>(1)) [linkage=external];
// IR-NEXT:     global %5 scalars: array<u64, 14> [storage=static] = aggregate<array<u64, 14>, zero_fill=false>(index0 = const<u64>(1), index1 = const<u64>(1), index2 = const<u64>(2), index3 = const<u64>(2), index4 = const<u64>(4), index5 = const<u64>(4), index6 = const<u64>(8), index7 = const<u64>(8), index8 = const<u64>(8), index9 = const<u64>(8), index10 = const<u64>(16), index11 = const<u64>(16), index12 = const<u64>(8), index13 = const<u64>(8)) [linkage=external];
// IR-NEXT:     global %9 object: @type0 [storage=static] [linkage=external];
// IR-NEXT:     fn %10 @objects(%11 pointer: ptr<atomic @type0>, %12 record: ptr<@type4>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(4), const<u64>(4)), const<u64>(4)), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
