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
// IR-NEXT:     type @type0 U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type1 S = struct {
// IR-NEXT:         field0 b: u32 : 3;
// IR-NEXT:         field1 a: array<i32, 2>;
// IR-NEXT:         field2 c: const i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 4, 16], bit_offsets=[Some(0), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// IR-NEXT:     type @type2 P = struct {
// IR-NEXT:         field0 x: f64;
// IR-NEXT:         field1 y: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type3 v4si = vector<i32, 4>;
// IR-NEXT:     type @type4 float4 = vector<f32, 4>;
// IR-NEXT:     type @type5 = enum : u32 {
// IR-NEXT:         %0 E0 = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %2 s: @type1 [storage=static] [linkage=external];
// IR-NEXT:     global %3 text: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// IR-NEXT:     global %16 flag: atomic bool [storage=static] [linkage=external];
// IR-NEXT:     global %17 counter: i32 [storage=static] [linkage=external];
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
// IR-NEXT:     fn %15 @make() -> @type2 [linkage=external] [abi=sysv64() -> native_c];
// IR-NEXT:     fn %18 @g(%19 c: i32, %20 p: ptr<i32>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %21 made: @type2 [storage=automatic] = copy<@type2, reason=assign>(call<@type2, signature=fn() -> @type2, abi=sysv64() -> native_c>(%15));
// IR-NEXT:         let %22 chosen: ptr<i32> [storage=automatic] = read<ptr<i32>>(%20);
// IR-NEXT:         let %23 nulled: ptr<i32> [storage=automatic] = read<ptr<i32>>(%20);
// IR-NEXT:         let %24 loaded: i32 [storage=automatic] = read<i32, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%17)));
// IR-NEXT:         let %25 fetched: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(1), const<i32>(0));
// IR-NEXT:         let %26 wide: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(3));
// IR-NEXT:         let %27 bits: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         return add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(field0(%21))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%22))))), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%23))))), widen<i64, reason=usual_arith>(read<i32>(%24))), widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(read<bool>(%25)))), read<i64>(%26)), widen<i64, reason=usual_arith>(read<i32>(%27))), reinterpret<i64, reason=explicit, fits=always>(const<u64>(4)));
// IR-NEXT:     }
// IR-NEXT:     fn %32 @h(%33 v: vector<i32, 4>, %34 q: vector<f32, 4>, %35 p: ptr<i32>) -> i32 [linkage=external] [abi=sysv64(direct, direct, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %36 sum: vector<i32, 4> [storage=automatic] = read<vector<i32, 4>>(%33);
// IR-NEXT:         let %38 value: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:         let %40 nulled: i32 [storage=automatic] = read<i32>(deref(conditional<ptr<i32>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<i32>>(%35), null<ptr<i32>>)));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(lane(%36, const<i32>(0))), read<i32>(%38)), read<i32>(%40));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
