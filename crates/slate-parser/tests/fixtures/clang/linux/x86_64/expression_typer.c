// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

union U { int i; float f; };
struct S { unsigned b : 3; int a[2]; const long c; } s;

_Static_assert(sizeof((char []){ "foo" }) == 4, "");
char text[] = { "ab" };
_Static_assert(sizeof text == 3, "");

int f(int n) {
    __typeof__(s.a) array;
    _Static_assert(sizeof array == 8, "");
    __typeof__(s.b + 0) promoted = 1;
    __auto_type sum = s.b + 1u;
    __typeof__(s.c) fixed = 2;
    __typeof__((const union U)1) cast;
    cast.i = n;
    __typeof__(&s.a[1]) element = &array[1];
    __typeof__(n ? 1.0f : 2) mixed = 0;
    int bits = _Generic(s.b + 0, int: 1, default: 2);
    return (int)(sizeof(n + 1L) + sizeof(s.a) + sizeof(sum) + sizeof(mixed)) + promoted + (int)fixed + cast.i + *element + bits;
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
// IR-NEXT:     type @type0 U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type1 S = struct {
// IR-NEXT:         field0 b: u32 : 3;
// IR-NEXT:         field1 a: array<i32, 2>;
// IR-NEXT:         field2 c: const i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 4, 16], bit_offsets=[Some(0), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// IR-NEXT:     global %2 s: @type1 [storage=static] [linkage=external];
// IR-NEXT:     global %3 text: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// IR-NEXT:     fn %4 @f(%5 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %6 array: array<i32, 2> [storage=automatic];
// IR-NEXT:         let %7 promoted: i32 [storage=automatic] = const<i32>(1);
// IR-NEXT:         let %8 sum: u32 [storage=automatic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%2)))), const<u32>(1));
// IR-NEXT:         let %9 fixed: i64 [storage=automatic] [const] = widen<i64, reason=assign>(const<i32>(2));
// IR-NEXT:         let %10 cast: @type0 [storage=automatic];
// IR-NEXT:         write<i32>(field0(%10), read<i32>(%5));
// IR-NEXT:         let %11 element: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%6), const<i32>(1))));
// IR-NEXT:         let %12 mixed: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// IR-NEXT:         let %13 bits: i32 [storage=automatic] = const<i32>(1);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), const<u64>(8)), const<u64>(4)), const<u64>(4)))), read<i32>(%7)), truncate<i32, reason=explicit, fits=unknown>(read<i64>(%9))), read<i32>(field0(%10))), read<i32>(deref(read<ptr<i32>>(%11)))), read<i32>(%13));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
