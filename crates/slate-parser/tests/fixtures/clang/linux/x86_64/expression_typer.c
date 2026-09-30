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

struct P { double x, y; };
struct P make(void);
_Atomic _Bool flag;
int counter;

long g(int c, int *p) {
    __typeof__(make()) made = make();
    _Static_assert(sizeof(make()) == 16, "");
    __typeof__(c ? p : 0) chosen = p;
    __typeof__(c ? p : (void *)0) nulled = p;
    __auto_type loaded = __atomic_load_n(&counter, 5);
    __typeof__(__c11_atomic_fetch_add(&flag, 1, 5)) fetched = 1;
    _Static_assert(sizeof(__builtin_complex(1.0f, 2.0f)) == 8, "");
    __typeof__(__builtin_choose_expr(1, 1L, 1.0)) wide = 3;
    __typeof__(__builtin_popcount(c)) bits = 0;
    return (long)made.x + *chosen + *nulled + loaded + fetched + wide + bits + (long)sizeof(__builtin_LINE());
}

typedef int v4si __attribute__((vector_size(16)));
typedef float float4 __attribute__((ext_vector_type(4)));
enum { E0 };

int h(v4si v, float4 q, int *p) {
    _Static_assert(sizeof(-v) == 16 && sizeof(~v) == 16 && sizeof(v[1]) == 4, "");
    _Static_assert(sizeof(q.xy) == 8 && sizeof(q.x) == 4 && sizeof(q.xxyy) == 16, "");
    _Static_assert(sizeof(__builtin_shufflevector(v, v, 0, 1)) == 8, "");
    _Static_assert(sizeof(__func__) == 2, "");
    __typeof__(v += 1) sum = v;
    __typeof__(({ int local = 1; local; })) value = 2;
    _Static_assert(sizeof(({ long wide = 0; wide; })) == 8, "");
    _Static_assert(_Generic(1 ? p : (void *)(1 - 1), int *: 1, default: 0), "");
    _Static_assert(_Generic(1 ? p : (void *)(E0 + 0), int *: 1, default: 0), "");
    _Static_assert(_Generic(1 ? p : (void *)(unsigned long)0, int *: 1, default: 0), "");
    _Static_assert(_Generic(1 ? p : (void *)(int)0.0, int *: 1, default: 0), "");
    _Static_assert(_Generic(1 ? p : (void *)(0, 0), void *: 1, default: 0), "");
    _Static_assert(_Generic(1 ? p : (void *)(int)(0.0 + 0.0), void *: 1, default: 0), "");
    int nulled = *(1 ? p : (void *)(1 - 1));
    return sum[0] + value + nulled;
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
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 b: u32 : 3;
// IR-NEXT:         field1 a: array<i32, 2>;
// IR-NEXT:         field2 c: const i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 4, 16], bit_offsets=[Some(0), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// IR-NEXT:         field0 x: f64;
// IR-NEXT:         field1 y: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// IR-NEXT:     type @type[[TYPE_float4:[0-9]+]] float4 = vector<f32, 4>;
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// IR-NEXT:         %[[VALUE_E0:[0-9]+]] E0 = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: atomic bool [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_array:[0-9]+]] array: array<i32, 2> [storage=automatic];
// IR-NEXT:         let %[[VALUE_promoted:[0-9]+]] promoted: i32 [storage=automatic] = const<i32>(1);
// IR-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: u32 [storage=automatic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_s]])))), const<u32>(1));
// IR-NEXT:         let %[[VALUE_fixed:[0-9]+]] fixed: i64 [storage=automatic] [const] = widen<i64, reason=assign>(const<i32>(2));
// IR-NEXT:         let %[[VALUE_cast:[0-9]+]] cast: @type[[TYPE_U]] [storage=automatic];
// IR-NEXT:         write<i32>(field0(%[[VALUE_cast]]), read<i32>(%[[VALUE_n]]));
// IR-NEXT:         let %[[VALUE_element:[0-9]+]] element: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_array]]), const<i32>(1))));
// IR-NEXT:         let %[[VALUE_mixed:[0-9]+]] mixed: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// IR-NEXT:         let %[[VALUE_bits:[0-9]+]] bits: i32 [storage=automatic] = const<i32>(1);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), const<u64>(8)), const<u64>(4)), const<u64>(4)))), read<i32>(%[[VALUE_promoted]])), truncate<i32, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_fixed]]))), read<i32>(field0(%[[VALUE_cast]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_element]])))), read<i32>(%[[VALUE_bits]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_make:[0-9]+]] @make() -> @type[[TYPE_P]] [linkage=external] [abi=sysv64() -> native_c];
// IR-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_made:[0-9]+]] made: @type[[TYPE_P]] [storage=automatic] = copy<@type[[TYPE_P]], reason=assign>(call<@type[[TYPE_P]], signature=fn() -> @type[[TYPE_P]], abi=sysv64() -> native_c>(%[[VALUE_make]]));
// IR-NEXT:         let %[[VALUE_chosen:[0-9]+]] chosen: ptr<i32> [storage=automatic] = read<ptr<i32>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE_nulled:[0-9]+]] nulled: ptr<i32> [storage=automatic] = read<ptr<i32>>(%[[VALUE_p]]);
// IR-NEXT:         let %[[VALUE_loaded:[0-9]+]] loaded: i32 [storage=automatic] = read<i32, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_counter]])));
// IR-NEXT:         let %[[VALUE_fetched:[0-9]+]] fetched: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(1), const<i32>(0));
// IR-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(3));
// IR-NEXT:         let %[[VALUE_bits_2:[0-9]+]] bits: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         return add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(field0(%[[VALUE_made]]))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_chosen]]))))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_nulled]]))))), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_loaded]]))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_fetched]])))), read<i64>(%[[VALUE_wide]])), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_bits_2]]))), reinterpret<i64, reason=explicit, fits=always>(const<u64>(4)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_h:[0-9]+]] @h(%[[VALUE_v:[0-9]+]] v: vector<i32, 4>, %[[VALUE_q:[0-9]+]] q: vector<f32, 4>, %[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [abi=sysv64(direct, direct, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_sum_2:[0-9]+]] sum: vector<i32, 4> [storage=automatic] = read<vector<i32, 4>>(%[[VALUE_v]]);
// IR-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:         let %[[VALUE_nulled_2:[0-9]+]] nulled: i32 [storage=automatic] = read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%[[VALUE_p_2]]), null<ptr<i32>>)));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(lane(%[[VALUE_sum_2]], const<i32>(0))), read<i32>(%[[VALUE_value]])), read<i32>(%[[VALUE_nulled_2]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
