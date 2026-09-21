// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
// SLATE-FILECHECK-STD DEFAULT c23

static_assert(_Generic(0L, long: 1, long long: 0));
static_assert(_Generic(0LL, long: 0, long long: 1));
static_assert(_Generic(1 < 2, int: 1, _Bool: 0));
static_assert(_Generic(!0, int: 1, _Bool: 0));
static_assert(_Generic(1 && 2, int: 1, _Bool: 0));
static_assert(_Generic((char)0, char: 1, signed char: 0, unsigned char: 0));
static_assert(_Generic(sizeof(int), unsigned long: 1, default: 0));
static_assert(_Generic(1L + 1LL, long long: 1, default: 0));
static_assert(_Generic((unsigned long)0 + (long long)0, unsigned long long: 1, default: 0));
static_assert(_Generic((unsigned _BitInt(64))0 + (long long)0, unsigned long long: 1, default: 0));

typeof(1L + 1LL) mixed_rank;
typeof(1L + 1u) signed_long;
typeof(1L < 1LL) comparison;
typeof(sizeof(int)) object_size;
const int qualified = 1;
typeof(&qualified) qualified_pointer;
struct Record { long long member; const int immutable; };

int identities(long l, long long ll, char c, signed char sc, int a, int b) {
    return _Generic(l + ll, long: 0, long long: 1)
        + _Generic(c, char: 1, signed char: 0)
        + _Generic(sc, char: 0, signed char: 1)
        + _Generic(a < b, int: 1, _Bool: 0)
        + _Generic(a && b, int: 1, _Bool: 0)
        + _Generic(!a, int: 1, _Bool: 0);
}

long long members(const struct Record *r, long l) {
    typeof(r->member + l) sum = r->member + l;
    typeof(&r->immutable) address = &r->immutable;
    return sum + *address;
}

long difference(const int *a, int *b) { return a - b; }
int truth_size(int a, int b) { return sizeof(a < b); }
int variadic(int tag, ...);
int half_argument(_Float16 value) { return variadic(0, value); }
typedef float float4 __attribute__((vector_size(16)));
typedef int int4 __attribute__((vector_size(16)));
int vector_mask(float4 a, float4 b) { return _Generic(a < b, int4: 1, default: 0); }
int array_compatibility(int (*p)[3]) { return _Generic(p, int (*)[]: 1, default: 0); }
int forwarded_truth(int a, int b) { return (a, a < b) && (b, a > b); }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 Record = struct {
// DEFAULT-NEXT:         field0 member: i64;
// DEFAULT-NEXT:         field1 immutable: const i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 float4 = vector<f32, 4> [c="float __attribute__((vector_size(16)))"];
// DEFAULT-NEXT:     type @type2 int4 = vector<i32, 4> [c="int __attribute__((vector_size(16)))"];
// DEFAULT-NEXT:     global %0 mixed_rank: i64 [storage=static] [linkage=external] [c="typeof(1L + 1LL)"] [c_canon="long long"];
// DEFAULT-NEXT:     global %1 signed_long: i64 [storage=static] [linkage=external] [c="typeof(1L + 1u)"] [c_canon="long"];
// DEFAULT-NEXT:     global %2 comparison: i32 [storage=static] [linkage=external] [c="typeof(1L < 1LL)"] [c_canon="int"];
// DEFAULT-NEXT:     global %3 object_size: u64 [storage=static] [linkage=external] [c="typeof(sizeof(...))"] [c_canon="unsigned long"];
// DEFAULT-NEXT:     global %4 qualified: i32 [storage=static] [const] = const<i32>(1) [linkage=external] [c="const int"] [c_const="true"];
// DEFAULT-NEXT:     global %5 qualified_pointer: ptr<const i32> [storage=static] [linkage=external] [c="typeof(&qualified)"] [c_canon="const int *"];
// DEFAULT-NEXT:     fn %7 @identities(%8 l: i64 [c="long"], %9 ll: i64 [c="long long"], %10 c: i8 [c="char"], %11 sc: i8 [c="signed char"], %12 a: i32 [c="int"], %13 b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(long, long long, char, signed char, int, int)"] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @members(%15 r: ptr<const @type0> [c="const struct Record *"], %16 l: i64 [c="long"]) -> i64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="long long"] [c="long long(const struct Record *, long)"] {
// DEFAULT-NEXT:         let %17 sum: i64 [storage=automatic] = add<i64, overflow=ub>(read<i64>(field0(deref(read<ptr<const @type0>>(%15)))), read<i64>(%16)) [c="typeof(r->member + l)"] [c_canon="long long"];
// DEFAULT-NEXT:         let %18 address: ptr<const i32> [storage=automatic] = addr_of<ptr<const i32>>(field1(deref(read<ptr<const @type0>>(%15)))) [c="typeof(&r->immutable)"] [c_canon="const int *"];
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%17), widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<const i32>>(%18)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @difference(%20 a: ptr<const i32> [c="const int *"], %21 b: ptr<i32> [c="int *"]) -> i64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="long"] [c="long(const int *, int *)"] {
// DEFAULT-NEXT:         return ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<const i32>>(%20), read<ptr<i32>>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @truth_size(%23 a: i32 [c="int"], %24 b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int)"] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4) [size_of="i32"]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @variadic(%38 tag: i32 [c="int"], ...) -> i32 [linkage=external] [c="int(int, ...)"];
// DEFAULT-NEXT:     fn %26 @half_argument(%27 value: f16 [c="_Float16"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(_Float16)"] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, ...) -> i32>(%25, const<i32>(0), read<f16>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @vector_mask(%31 a: vector<f32, 4> [c="float4"] [c_canon="float __attribute__((vector_size(16)))"] [typedef_chain="float4"], %32 b: vector<f32, 4> [c="float4"] [c_canon="float __attribute__((vector_size(16)))"] [typedef_chain="float4"]) -> i32 [linkage=external] [abi=sysv64(direct, direct) -> scalar] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(float4, float4)"] [c_canon="int(float __attribute__((vector_size(16))), float __attribute__((vector_size(16))))"] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @array_compatibility(%34 p: ptr<array<i32, 3>> [c="int (*)[3]"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int (*)[3])"] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @forwarded_truth(%36 a: i32 [c="int"], %37 b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int)"] {
// DEFAULT-NEXT:         read<i32>(%36);
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%36), read<i32>(%37))
// DEFAULT-NEXT:             read<i32>(%37);
// DEFAULT-NEXT:             write<bool>(%39, gt<i32>(read<i32>(%36), read<i32>(%37)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(false));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%39));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
